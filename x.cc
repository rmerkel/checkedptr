#include "CheckedPtr.h"

int main() {
	int a[100];
	CheckedPtr<int> cpi(a, a, 100);

	for (int i = 0; i < 100; ++i)
		cpi[i] = i;

	return 0;
}
