#include "mpi.h"
MPL_BEGIN
void mpi::print(std::string_view str) const
{
	std::cout << str << std::endl;
	std::cout << "sign :" << sign << std::endl;
	std::cout << "alloc:" << alloc << std::endl;
	std::cout << "used :" << used << std::endl;
	std::cout << "{ ";
	for (size_t i = 0; i < used; i++)
	{
		std::cout << dp[i] << ", ";
	}
	std::cout << "}" << std::endl;
}
MPL_END