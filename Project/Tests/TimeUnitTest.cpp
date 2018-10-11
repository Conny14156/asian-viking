#ifdef _UNITTESTS_

#include "CppUnitTest.h"

#include <Core\Time.hpp>
#include <Core\EngineModule\EngineModuleManager.hpp>
#include <thread>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(TimeUnitTest)
	{
	public:


		TEST_METHOD(TestTime)
		{
			Assert::IsFalse(Engine::GetModule<Time>().time("test", 5));
			std::this_thread::sleep_for(std::chrono::seconds(1));
			Assert::IsTrue(Engine::GetModule<Time>().time("test", 1));
		}
	};

}

#endif