#include "mpi.h"
#include <chrono>
#include <iomanip>
#include <fstream>
MPL_BEGIN

namespace tune
{
	static std::chrono::steady_clock::time_point time_now()
	{
		return std::chrono::steady_clock::now();
	}

	static std::chrono::steady_clock::time_point TIME_POINT_TMP;
	static void timer_start()
	{
		TIME_POINT_TMP = time_now();
	}

	/* 返回纳秒 */
	static uint64_t timer_stop()
	{
		return std::chrono::duration_cast
			<std::chrono::nanoseconds>
			(time_now() - TIME_POINT_TMP).count();
	}

	static bool CHECK_RESULT;
	static size_t N_OF_TEST_LOOPS;
	static int STABILIZATION_EXTRA; /* 稳定 */
	static size_t OFFSET = 1;

	static uint64_t time_mul(size_t opnd_length)
	{
		mpi a = mpi::rand(opnd_length * OFFSET);
		mpi b = mpi::rand(opnd_length);
		mpi c, d;
		uint64_t t1 = 0;
		timer_start();
		for (size_t i = 0; i < N_OF_TEST_LOOPS; i++)
		{
			mpi::mul(a, b, c);
			if (CHECK_RESULT)
				/* 这里使用基线乘法来验证结果是否正确，
				而基线乘法相当慢，所以尽量不要 CHECK_RESULT
				要验证算法正确性请使用 mpi_test 。 */
			{
				mpi::baseline_mul(a, b, d);
				if (mpi::cmp(c, d) != ord::eq)
				{
					return 0; /* 因为用时不可能为 0 ，所以用此标记出错了 */
				}
			}
		}
		t1 = timer_stop();
		return t1;
	}

	static uint64_t time_sqr(size_t opnd_length)
	{
		mpi a = mpi::rand(opnd_length);
		mpi b, c;
		uint64_t t1;

		timer_start();
		for (size_t i = 0; i < N_OF_TEST_LOOPS; i++)
		{
			mpi::sqr(a, b);
			if (CHECK_RESULT)
				/* 这里使用基线平方来验证结果是否正确，
				而基线乘法相当慢，所以尽量不要 CHECK_RESULT
				要验证算法正确性请使用 mpi_test 。 */
			{
				mpi::baseline_sqr(a, c);
				if (mpi::cmp(b, c) != ord::eq)
				{
					return 0; /* 因为用时不可能为 0 ，所以用此标记出错了 */
				}
			}
		}
		t1 = timer_stop();
		return t1;
	}

	static struct tune_args
	{
		int test_mode;
		int verbose; /* 冗长的 */
		int print;   /* 是否输出到文件 */
		int bncore;
		int terse;  /* 简要的 */
		int upper_limit_print;
		int increment_print;
	} TUNE_ARGS;

	static void run(std::string_view name, uint64_t(*op)(size_t), int* cutoff)
	{
		if (TUNE_ARGS.verbose == 1 || TUNE_ARGS.test_mode == 1)
		{
			std::cout << "# " << name << "." << std::endl;
		}
		uint64_t t1, t2;
		int x, count = 0;
		for (x = 8; x < TUNE_ARGS.upper_limit_print; x += TUNE_ARGS.increment_print)
		{
			*cutoff = INT_MAX;
			t1 = op(x);
			if (t1 == 0ull || t1 == UINT64_MAX)
			{
				std::cerr << name << " failed at x = INT_MAX ("
					<< (t1 == 0ull ? "wrong result" : "internal error")
					<< ")" << std::endl;
				exit(EXIT_FAILURE);
			}
			*cutoff = x;
			t2 = op(x);
			if (t2 == 0ull || t2 == UINT64_MAX)
			{
				std::cerr << name << " failed at x = INT_MAX ("
					<< (t2 == 0ull ? "wrong result" : "internal error")
					<< ")" << std::endl;
				exit(EXIT_FAILURE);
			}
			if (TUNE_ARGS.verbose == 1)
			{
				std::cout << x << ": "
					<< std::setw(9) << t1 << " "
					<< std::setw(9) << t2 << ", "
					<< std::setw(9) << int64_t(t2 - t1) << std::endl;
			}
			if (t2 < t1)
			{
				if (count == STABILIZATION_EXTRA)
				{
					count = 0; break;
				}
				else if (count < STABILIZATION_EXTRA)
				{
					count++;
				}
			}
			else if (count > 0)
			{
				count--;
			}
		}
		*cutoff = x - STABILIZATION_EXTRA * TUNE_ARGS.increment_print;
	}

	static long str_to_long(const char* str, char** endptr, const char* err)
	{
		static const int base = 10;
		char* _endptr;
		long val;
		errno = 0;
		val = strtol(str, &_endptr, base);
		if ((val > INT_MAX || val < 0) || (errno != 0))
		{
			std::cerr << "Value " << err << " is not usable." << std::endl;
			exit(EXIT_FAILURE);
		}
		if (_endptr == str)
		{
			std::cerr << err << std::endl;
		}
		if (endptr != nullptr)
		{
			*endptr = _endptr;
		}
		return val;
	}

	static int EXIT_CODE = EXIT_FAILURE;
	static void usage(const char* s)
	{
		std::cerr << "Usage: " << s << "[TvcpGbtrSLFfMmosh]" << std::endl;
		std::cerr << "          -T testmode, for use with testme.sh" << std::endl;
		std::cerr << "          -v verbose, print all timings" << std::endl;
		std::cerr << "          -c check results" << std::endl;
		std::cerr << "          -p print benchmark of final cutoffs in files \"multiplying\"" << std::endl;
		std::cerr << "             and \"squaring\"" << std::endl;
		std::cerr << "          -G [string] suffix for the filenames listed above" << std::endl;
		std::cerr << "             Implies '-p'" << std::endl;
		std::cerr << "          -b print benchmark of bncore.c" << std::endl;
		std::cerr << "          -t prints space (0x20<<std::endl separated results" << std::endl;
		std::cerr << "          -r [64] number of rounds" << std::endl;
		std::cerr << "          -S [0xdeadbeef] seed for PRNG" << std::endl;
		std::cerr << "          -L [3] number of negative values accumulated until the result is accepted" << std::endl;
		std::cerr << "          -M [3000] upper limit of T-C tests/prints" << std::endl;
		std::cerr << "          -m [1] increment of T-C tests/prints" << std::endl;
		std::cerr << "          -o [1] multiplier for the second multiplicand" << std::endl;
		std::cerr << "             (Not for computing the cut-offs!<<std::endl" << std::endl;
		std::cerr << "          -s 'preset' use values in 'preset' for printing." << std::endl;
		std::cerr << "             'preset' is a comma separated string with cut-offs for" << std::endl;
		std::cerr << "             ksm, kss, tc3m, tc3s in that order" << std::endl;
		std::cerr << "             ksm  = karatsuba multiplication" << std::endl;
		std::cerr << "             kss  = karatsuba squaring" << std::endl;
		std::cerr << "             tc3m = Toom-Cook 3-way multiplication" << std::endl;
		std::cerr << "             tc3s = Toom-Cook 3-way squaring" << std::endl;
		std::cerr << "             Implies '-p'" << std::endl;
		std::cerr << "          -h this message" << std::endl;
		exit(EXIT_CODE);
	}

	struct cutoffs
	{
		int KARATSUBA_MUL;
		int KARATSUBA_SQR;
		int TOOM_COOK_MUL;
		int TOOM_COOK_SQR;
	};

	static const cutoffs MAX_CUTOFFS = { INT_MAX, INT_MAX, INT_MAX, INT_MAX };

	static void set_cutoffs(cutoffs const& c)
	{
		MPI_KARA_MUL_CUTOFF = c.KARATSUBA_MUL;
		MPI_KARA_SQR_CUTOFF = c.KARATSUBA_SQR;
		MPI_TOOM_MUL_CUTOFF = c.TOOM_COOK_MUL;
		MPI_TOOM_SQR_CUTOFF = c.TOOM_COOK_SQR;
	}

	static void get_cutoffs(cutoffs& c)
	{
		c.KARATSUBA_MUL = MPI_KARA_MUL_CUTOFF;
		c.KARATSUBA_SQR = MPI_KARA_SQR_CUTOFF;
		c.TOOM_COOK_MUL = MPI_TOOM_MUL_CUTOFF;
		c.TOOM_COOK_SQR = MPI_TOOM_SQR_CUTOFF;
	}

	static int _main_(int argc, char** argv)
	{
		int printpreset = 0;
		uint64_t seed = time_now().time_since_epoch().count();

		char mullog[256] = "multiplying";
		char sqrlog[256] = "squaring";
		N_OF_TEST_LOOPS = 64;
		STABILIZATION_EXTRA = 3;

		TUNE_ARGS.test_mode = 0;
		TUNE_ARGS.verbose = 0;
		TUNE_ARGS.print = 0;
		TUNE_ARGS.bncore = 0;
		TUNE_ARGS.terse = 0;
		TUNE_ARGS.upper_limit_print = 3000;
		TUNE_ARGS.increment_print = 1;

		int i, j, n, x;
		char* str, * endptr;
		uint64_t t1, t2;

		if (argc != 1)
		{
			for (int opt = 1; opt < argc && argv[opt][0] == '-'; opt++)
			{
				switch (argv[opt][1])
				{
				case 'T':
					TUNE_ARGS.test_mode = 1;
					CHECK_RESULT = true;
					TUNE_ARGS.upper_limit_print = 1000;
					TUNE_ARGS.increment_print = 11;
					N_OF_TEST_LOOPS = 1;
					STABILIZATION_EXTRA = 1;
					OFFSET = 1;
					break;
				case 'v':
					TUNE_ARGS.verbose = 1;
					break;
				case 'c':
					CHECK_RESULT = true;
					break;
				case 'p':
					TUNE_ARGS.print = 1;
					break;
				case 'G':
					TUNE_ARGS.print = 1;
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					/* 获取长度，相当于 strcat() */
					for (i = 0; i < 255; i++)
					{
						if (mullog[i] == '\0')
						{
							break;
						}
					}
					for (j = 0; i < 255; j++, i++)
					{
						sqrlog[i] = argv[opt][j];
						if (argv[opt][j] == '\0')
						{
							break;
						}
					}
					break;
				case 'b':
					TUNE_ARGS.bncore = 1;
					break;
				case 't':
					TUNE_ARGS.terse = 1;
					break;
				case 'S':
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					str = argv[opt];
					errno = 0;
					seed = uint64_t(str_to_long(argv[opt], nullptr, "No seed given?\n"));
					break;
				case 'L':
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					STABILIZATION_EXTRA = int(str_to_long(argv[opt], nullptr, "No value for option \"-L\"given"));
					break;
				case 'o':
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					OFFSET = int(str_to_long(argv[opt], nullptr, "No value for the offset given"));
					break;
				case 'r':
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					N_OF_TEST_LOOPS = int(str_to_long(argv[opt], nullptr, "No value for the number of rounds given"));
					break;

				case 'M':
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					TUNE_ARGS.upper_limit_print = (int)str_to_long(argv[opt], nullptr, "No value for the upper limit of T-C tests given");
					break;
				case 'm':
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					TUNE_ARGS.increment_print = (int)str_to_long(argv[opt], nullptr, "No value for the increment for the T-C tests given");
					break;
				case 's':
					printpreset = 1;
					TUNE_ARGS.print = 1;
					opt++;
					if (opt >= argc)
					{
						usage(argv[0]);
					}
					str = argv[opt];
					MPI_KARA_MUL_CUTOFF = (int)str_to_long(str, &endptr, "[1/4] No value for KARATSUBA_MUL_CUTOFF given");
					str = endptr + 1;
					MPI_KARA_SQR_CUTOFF = (int)str_to_long(str, &endptr, "[2/4] No value for KARATSUBA_SQR_CUTOFF given");
					str = endptr + 1;
					MPI_TOOM_MUL_CUTOFF = (int)str_to_long(str, &endptr, "[3/4] No value for TOOM_MUL_CUTOFF given");
					str = endptr + 1;
					MPI_TOOM_SQR_CUTOFF = (int)str_to_long(str, &endptr, "[4/4] No value for TOOM_SQR_CUTOFF given");
					break;
				case 'h':
					EXIT_CODE = EXIT_SUCCESS;
					/* FALLTHROUGH */
				default:
					usage(argv[0]);
				}
			}
		}

		mpi::srand(seed);

		cutoffs original, updated;
		get_cutoffs(original);
		updated = MAX_CUTOFFS;

		if (TUNE_ARGS.bncore == 0 && printpreset == 0)
		{
			struct
			{
				const char* name;
				int* cutoff, * update;
				uint64_t(*fn)(size_t);
			} test[] =
			{
				/* 需要手动屏蔽 comba_mul 或者 comba_sqr 的调用 */
				{
					.name = "\"Karatsuba multiplication\"",
					.cutoff = &MPI_KARA_MUL_CUTOFF,
					.update = &updated.KARATSUBA_MUL,
					.fn = time_mul
				},
				{
					.name = "\"Karatsuba squaring\"",
					.cutoff = &MPI_KARA_SQR_CUTOFF,
					.update = &updated.KARATSUBA_SQR,
					.fn = time_sqr
				},
				{
					.name = "\"Toom-Cook 3-way multiplying\"",
					.cutoff = &MPI_TOOM_MUL_CUTOFF,
					.update = &updated.TOOM_COOK_MUL,
					.fn = time_mul
				},
				{
					.name = "\"Toom-Cook 3-way squaring\"",
					.cutoff = &MPI_TOOM_SQR_CUTOFF,
					.update = &updated.TOOM_COOK_SQR,
					.fn = time_sqr
				}
			};

			set_cutoffs(MAX_CUTOFFS);
			for (n = 0; n < sizeof(test) / sizeof(test[0]); n++)
			{
				if (test[n].fn)
				{
					run(test[n].name, test[n].fn, test[n].cutoff);
					*test[n].update = *test[n].cutoff;
					*test[n].cutoff = INT_MAX;
				}
			}
		}
		if (TUNE_ARGS.terse == 1)
		{
			std::cout << updated.KARATSUBA_MUL << " "
				<< updated.KARATSUBA_SQR << " "
				<< updated.TOOM_COOK_MUL << " "
				<< updated.TOOM_COOK_SQR << " " << std::endl;
		}
		else
		{
			std::cout << "KARATSUBA_MUL_CUTOFF = " << updated.KARATSUBA_MUL << std::endl
				<< "KARATSUBA_SQR_CUTOFF = " << updated.KARATSUBA_SQR << std::endl
				<< "TOOM_COOK_MUL_CUTOFF = " << updated.TOOM_COOK_MUL << std::endl
				<< "TOOM_COOK_SQR_CUTOFF = " << updated.TOOM_COOK_SQR << std::endl;
		}

		if (TUNE_ARGS.print == 1)
		{
			std::ofstream multiplying(mullog);
			if (!multiplying.is_open())
			{
				std::cerr << "Opening file \"" << mullog << "\" failed" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::ofstream squaring(sqrlog);
			if (!squaring.is_open())
			{
				std::cerr << "Opening file \"" << sqrlog << "\" failed" << std::endl;
				exit(EXIT_FAILURE);
			}
			// line. 502
			for (x = 8; x < TUNE_ARGS.upper_limit_print; x += TUNE_ARGS.increment_print)
			{
				set_cutoffs(MAX_CUTOFFS);
				t1 = time_mul(x);
				set_cutoffs(original);
				t2 = time_mul(x);
				multiplying << x << ": "
					<< std::setw(9) << t1 << " "
					<< std::setw(9) << t2 << ", "
					<< std::setw(9) << int64_t(t2 - t2) << std::endl;
				if (TUNE_ARGS.verbose == 1)
				{
					std::cout << "MUL " << x << ": "
						<< std::setw(9) << t1 << " "
						<< std::setw(9) << t2 << ", "
						<< std::setw(9) << int64_t(t2 - t1) << std::endl;
				}
				set_cutoffs(MAX_CUTOFFS);
				t1 = time_sqr(x);
				set_cutoffs(original);
				t2 = time_sqr(x);
				squaring << x << ": "
					<< std::setw(9) << t1 << " "
					<< std::setw(9) << t2 << ", "
					<< std::setw(9) << int64_t(t2 - t2) << std::endl;
				if (TUNE_ARGS.verbose == 1)
				{
					std::cout << "SQR " << x << ": "
						<< std::setw(9) << t1 << " "
						<< std::setw(9) << t2 << ", "
						<< std::setw(9) << int64_t(t2 - t1) << std::endl;
				}
			}
			std::cout << "Finished. Data for graphing in \""
				<< mullog << "\" and \"" << sqrlog << "\"" << std::endl;
			if (TUNE_ARGS.verbose == 1)
			{
				set_cutoffs(original);
				if (TUNE_ARGS.terse == 1)
				{
					std::cout << MPI_KARA_MUL_CUTOFF << " "
						<< MPI_KARA_SQR_CUTOFF << " "
						<< MPI_TOOM_MUL_CUTOFF << " "
						<< MPI_TOOM_SQR_CUTOFF << " " << std::endl;
				}
				else
				{
					std::cout << "KARATSUBA_MUL_CUTOFF = " << MPI_KARA_MUL_CUTOFF << std::endl;
					std::cout << "KARATSUBA_SQR_CUTOFF = " << MPI_KARA_SQR_CUTOFF << std::endl;
					std::cout << "TOOM_COOK_MUL_CUTOFF = " << MPI_TOOM_MUL_CUTOFF << std::endl;
					std::cout << "TOOM_COOK_SQR_CUTOFF = " << MPI_TOOM_SQR_CUTOFF << std::endl;
				}
			}
		}
		return EXIT_SUCCESS;
	}
}

MPL_END