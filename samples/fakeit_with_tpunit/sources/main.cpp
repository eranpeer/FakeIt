#include "fakeit.hpp"
#include "TpUnitAdapter.hpp"
#include "tpunit++.hpp"

using namespace fakeit;

int main() {
	TpUnitAdapter adapter;
	Fakeit.setTestingFrameworkAdapter(adapter);
	return tpunit::Tests::Run();
}
