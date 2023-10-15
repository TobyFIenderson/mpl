import math


MPI_DIGIT_BITS = 28
MPI_BASEX = 1 << MPI_DIGIT_BITS
MPI_MASK = MPI_BASEX - 1

def get_mpi_from_file(filepath):
    with open(filepath) as file_object:
        contents = file_object.read()
    lines = contents.split('\n')
    lines.pop(len(lines) - 1)
    dps = []
    for line in lines:
        dp0 = line.split(' ')
        dp1 = []
        for num in dp0:
            dp1.append(int(num))
        dps.append(dp1)

    mpis = []
    for dp in dps:
        sign = dp[0]
        dp.pop(0)
        exp = 1
        d = 0
        for i in dp:
            d += i * exp
            exp *= MPI_BASEX
        d = d * sign
        mpis.append(d)
    return mpis


def write_file(filepath, contests):
    with open(filepath, "w") as f:
        f.write(contests)


def test_gcd(opnd1_file_path, opnd2_file_path, in_result_file_path, out_result_file_path):
    opnd1 = get_mpi_from_file(opnd1_file_path)
    opnd2 = get_mpi_from_file(opnd2_file_path)
    presult = []
    for i in range(len(opnd1)):
        presult.append(math.gcd(opnd1[i], opnd2[i]))
    cresult = get_mpi_from_file(in_result_file_path)
    ss = ""
    for i in range(len(presult)):
        s = ""
        s += str(i + 1)
        s += ". "
        b = presult[i] == cresult[i]
        s += str(b)
        if b == False :
            s += "\n  C++: "
            s += str(cresult[i])
            s += "\n  Py : "
            s += str(presult[i])
        print(s)
        s += '\n'
        ss += s
    write_file(out_result_file_path, ss)


test_gcd("mpi_test/gcd_test_operand_1", "mpi_test/gcd_test_operand_2", "mpi_test/gcd_test_result", "mpi_test/gcd_test_py_result")