#include <iostream> 
#include <thread>
#include <cmath>
#include <ctime>
#include <iomanip>

using namespace std;

long double Sigma(const long &num_th, long constant);
unsigned long long BBP(const long &n_th);
long PowMod(long base, long exp, long numMod);

int main(int argc, char* argv[])
{
	int numThread = 0, numDigit = 0, PIsize = 0, amount = 0;
	unsigned long long *PI = nullptr;
	double mytime = static_cast<double>(clock());
	thread *myThreads = nullptr;
	/*argc = 3;
	argv[1] = "2345";
	argv[2] = "4";*/
	if (argc != 3) {
		cout << "wrong input\n";
		return -1;
	}
	for (int i = 0; argv[1][i] != '\0'; ++i) {
		numDigit *= 10;
		numDigit += argv[1][i] - '0';
	}
	for (int i = 0; argv[2][i] != '\0'; ++i) {
		numThread *= 10;
		numThread += argv[2][i] - '0';
	}
	PIsize = numDigit / 9 + ((numDigit % 9 == 0) ? 0 : 1);
	if (PIsize == 0) {
		PIsize = 1;
	}
	if (numThread > PIsize) {
		numThread = PIsize;
	}
	//PI = (decltype(PI))malloc(sizeof(unsigned long long) * PIsize);
	PI = new unsigned long long[PIsize];
	myThreads = new thread[numThread];
	amount = PIsize / numThread;
	if (amount == 0) {
		amount = 1;
	}
	for (int i = 0, PI_index = 0; i < numThread; ++i, PI_index += amount) {
		myThreads[i] = thread([&PI, PI_index, amount](){
			for (int j = PI_index; j < PI_index + amount; ++j) {
				PI[j] = BBP(j * 8);
			}
		});
	}
	if (PIsize % numThread != 0) {
		PI[PIsize - 1] = BBP((PIsize - 1) * 8);
	}
	cout << "PI = 3.";
	for (int i = 0, k = 0; i < numThread; ++i) {
		myThreads[i].join();
		/*for (int j = 0; j < amount; ++j, ++k) {
		cout << ((k % 8 == 0) ? "\n" : " ") << PI[k];
		}*/
	}
	//cout << " " << (PI[PIsize - 1] >> (4 * (8 - numDigit % 8)));
	for (int i = 0, carry = 0; i < PIsize - 1; ++i) {
		for (int j = PIsize - 1; j >= 0; --j) {
			PI[j] = PI[j] * 1000000000 + carry;
			carry = PI[j] >> 32;
			PI[j] &= 0xffffffff;
		}
		cout << ((i % 8 == 0) ? "\n" : " ") << setw(9) << setfill('0') << carry;
	}
	cout << " ";
	for (int i = numDigit % 9, carry = 0; i > 0; --i) {
		PI[0] = PI[0] * 10 + carry;
		carry = PI[0] >> 32;
		PI[0] &= 0xffffffff;
		cout << carry;
	}
	mytime = static_cast<double>(clock() - mytime);
	cout << "\n\n(" << mytime << " ms, calculate with " << numThread << " threads)\n";
	//free(PI);
	delete[] PI;
	delete[] myThreads;
	//cin.get();
	return 0;
}

long double Sigma(const long &num_th, long constant)
{
	long double kn_result = 0.0, oo_result = 0.0, numerator = 1;
	long &denominator = constant;
	for (register int k = 0; k <= num_th; ++k, denominator += 8) {
		kn_result += static_cast<long double>(PowMod(16, num_th - k, denominator)) / static_cast<long double>(denominator);
		if (kn_result >= 1) {
			kn_result = fmod(kn_result, 1);
		}
	}
	while (true)
	{
		numerator /= 16;
		oo_result = kn_result + numerator / static_cast<long double>(denominator);
		if (kn_result == oo_result) {
			break;
		}
		kn_result = oo_result;
		denominator += 8;
	}

	return oo_result;
}

unsigned long long BBP(const long &n_th)
{
	long double ans_hex = fmod(4 * Sigma(n_th, 1) - 2 * Sigma(n_th, 4) - Sigma(n_th, 5) - Sigma(n_th, 6), 1);
	if (ans_hex < 0) {
		ans_hex += 1;
	}
	return static_cast<unsigned long long>(ans_hex * 0x100000000);
}

long PowMod(long base, long exp, long numMod)
{
	long ans = 1;
	base %= numMod;
	while (true)
	{
		if ((exp & 1) == 1) {
			ans = (ans * base) % numMod;
		}
		if ((exp >>= 1) == 0) {
			break;
		}
		base = (base * base) % numMod;
	}
	return ans;
}
