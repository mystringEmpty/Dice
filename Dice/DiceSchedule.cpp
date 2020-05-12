#include <mutex>
#include <condition_variable>
#include <deque>
#include "DiceJob.h" 

map<string, cmd> mCommand = {
	{"reload",cq_restart},
	{"die",cq_exit},
	{"update",dice_update}
};

void DiceJob::exec() {
	if (auto it = mCommand.find(cmd_key); it != mCommand.end()) {
		it->second(*this);
	}
	else return;
}
void DiceJob::echo(const std::string& msg) {
	switch (fromChat.second) {
	case CQ::msgtype::Private:
		CQ::sendPrivateMsg(fromQQ, msg);
		break;
	case CQ::msgtype::Group:
		CQ::sendGroupMsg(fromChat.first, msg);
		break;
	case CQ::msgtype::Discuss:
		CQ::sendDiscussMsg(fromChat.first, msg);
		break;
	}
}
void DiceJob::note(const std::string& strMsg, int note_lv = 0b1) {
	ofstream fout(string("DiceData\\audit\\log") + to_string(console.DiceMaid) + "_" + printDate() + ".txt", ios::out | ios::app);
	fout << printSTNow() << "\t" << note_lv << "\t" << printLine(strMsg) << std::endl;
	fout.close();
	echo(strMsg);
	string note = getName(fromQQ) + strMsg;
	for (const auto& [ct, level] : console.NoticeList) {
		if (!(level & note_lv) || pair(fromQQ, CQ::msgtype::Private) == ct || ct == fromChat)continue;
		AddMsgToQueue(note, ct);
	}
}

// 待处理任务队列
std::queue<DiceJobDetail> queueJob;
std::mutex mtQueueJob;
std::condition_variable cvJob;
void jobHandle() {
	while (Enabled) {
		if (std::unique_lock<std::mutex> lock_queue(mtQueueJob); !queueJob.empty()) {
			DiceJob job(queueJob.front());
			queueJob.pop();
			lock_queue.unlock();
			job.exec();
		}
		else {
			cvJob.wait(lock_queue, []() {return !queueJob.empty(); });
		}
	}
}
void DiceScheduler::push_job(DiceJobDetail& job) {
	if (!Enabled)return;
	std::unique_lock<std::mutex> lock_queue(mtQueueJob);
	queueJob.push(job);
	lock_queue.unlock();
	cvJob.notify_one();
}
std::unique_ptr<std::thread> threadJobs;

//延时任务队列
using waited_job = pair< time_t, DiceJobDetail>;
std::priority_queue<DiceJobDetail, std::deque<DiceJobDetail>, std::deque<DiceJobDetail>> queueJobWaited;
std::mutex mtJobWaited;

void DiceScheduler::start() {
	threadJobs = std::make_unique<std::thread>(jobHandle);
	threadJobs->detach();
}
void DiceScheduler::end() {
	threadJobs.reset();
}


