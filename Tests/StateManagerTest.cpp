#include <string>
#include <vector>
#include "doctest.h"
#include "State/StateManager.h"

namespace
{
	//ŒÄ‚Î‚ê‚½‚±‚Æ‚ğ‡”Ô‚É‹L˜^‚·‚éó‘Ô
	class RecordingState : public StateBase
	{
		std::string name_;
		std::vector<std::string>* pLog_;

	public:
		RecordingState(StateManager* pManager, const std::string& name, std::vector<std::string>* pLog)
			:StateBase(pManager), name_(name), pLog_(pLog)
		{
		}
		~RecordingState() override { pLog_->push_back(name_ + ":Destroyed"); }
		void EnterState() override { pLog_->push_back(name_ + ":Enter"); }
		void UpdateState() override { pLog_->push_back(name_ + ":Update"); }
		void ExitState() override { pLog_->push_back(name_ + ":Exit"); }
	};

	using Log = std::vector<std::string>;

	//‹L˜^‚ğ1‚Â‚Ì•¶š—ñ‚É‚·‚éi¸”s‚µ‚½‚Æ‚«‚É“Ç‚İ‚â‚·‚¢‚æ‚¤‚ÉA”ä‚×‚é‚Ì‚Í‚±‚Ì•¶š—ñj
	std::string Join(const Log& log)
	{
		std::string text;
		for (const std::string& entry : log)
		{
			if (!text.empty())	text += " ";
			text += entry;
		}
		return text;
	}
}

TEST_CASE("StateManager: Update does nothing before any state is entered")
{
	Log log;
	{
		StateManager manager(nullptr);
		manager.AddState("A", new RecordingState(&manager, "A", &log));
		manager.Update();
		CHECK(log.empty());
	}
}

TEST_CASE("StateManager: ChangeState exits the old state, then enters the new one, and Update goes to the current state")
{
	Log log;
	{
		StateManager manager(nullptr);
		manager.AddState("A", new RecordingState(&manager, "A", &log));
		manager.AddState("B", new RecordingState(&manager, "B", &log));

		manager.ChangeState("A");
		manager.Update();
		manager.ChangeState("B");
		manager.Update();

		CHECK(Join(log) == "A:Enter A:Update A:Exit B:Enter B:Update");
	}
}

TEST_CASE("StateManager: ChangeState to an unknown name does nothing")
{
	Log log;
	{
		StateManager manager(nullptr);
		manager.AddState("A", new RecordingState(&manager, "A", &log));
		manager.ChangeState("A");
		log.clear();

		manager.ChangeState("unknown");
		manager.Update();
		CHECK(Join(log) == "A:Update");
	}
}

TEST_CASE("StateManager: changing to the current state exits and enters it again")
{
	Log log;
	{
		StateManager manager(nullptr);
		manager.AddState("A", new RecordingState(&manager, "A", &log));
		manager.ChangeState("A");
		manager.ChangeState("A");
		CHECK(Join(log) == "A:Enter A:Exit A:Enter");
	}
}

TEST_CASE("StateManager: AddState with an existing name returns the existing state")
{
	Log log;
	Log duplicateLog;
	{
		StateManager manager(nullptr);
		StateBase* first = manager.AddState("A", new RecordingState(&manager, "A", &log));

		//“¯‚¶–¼‘O‚Å“n‚µ‚½‚à‚Ì‚Í“o˜^‚³‚ê‚¸AŠÇ—‚à‚³‚ê‚È‚¢ió‚¯æ‚Á‚½‘¤‚ÅÁ‚·‚±‚Æ‚É‚È‚éj
		RecordingState* duplicate = new RecordingState(&manager, "A", &duplicateLog);
		CHECK(manager.AddState("A", duplicate) == first);
		delete duplicate;

		manager.ChangeState("A");
		CHECK(Join(log) == "A:Enter");
		CHECK(duplicateLog.size() == 1);	//Á‚µ‚½‚Æ‚«‚Ì"A:Destroyed"‚¾‚¯
	}
}

TEST_CASE("StateManager: the destructor exits and deletes every registered state in name order, even ones never entered")
{
	Log log;
	{
		StateManager manager(nullptr);
		manager.AddState("C", new RecordingState(&manager, "C", &log));
		manager.AddState("A", new RecordingState(&manager, "A", &log));
		manager.AddState("B", new RecordingState(&manager, "B", &log));
		manager.ChangeState("B");
		log.clear();
	}
	CHECK(Join(log) == "A:Exit A:Destroyed B:Exit B:Destroyed C:Exit C:Destroyed");
}

TEST_CASE("StateManager: GetGameObject returns the owner it was made with")
{
	StateManager manager(nullptr);
	CHECK(manager.GetGameObject() == nullptr);
}
