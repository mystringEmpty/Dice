#pragma once

/**
 * ��������ϸ
 * �����ݿ�ʽ�Ĺ���
 * Copyright (C) 2019-2020 String.Empty
 */

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

using std::pair;
using std::string;
using std::to_string;
using std::vector;
using std::map;
using std::multimap;
using std::set;
using std::unordered_map;
using std::unordered_set;

class FromMsg;

enum class BlackID { fromGroup, fromQQ, inviterQQ, ownerQQ };

class DDBlackMark
{
	//������¼����:null,kick,ban,spam,other,ruler,local,extern;ö��֮��һ�ɷǷ�
	string type = "null";
	string time;
	//fromGroup,fromQQ,inviterQQ,ownerQQ
	using item = pair<long long, bool>;
	item fromGroup{0, false};
	item fromQQ{0, false};
	item inviterQQ{0, false};
	item ownerQQ{0, false};
	//
	long long DiceMaid = 0;
	long long masterQQ = 0;
	//Σ�յȼ�
	short danger = 0;
	//Ψһ�ƶ˱�ţ�0��ʾ����Ӧ�ƶˣ�
	int wid = 0;
	//˵��
	string note;
	//���ӱ�ע
	string comment{};
	void erase();
	void upload();
	int check_cloud();
public:
	DDBlackMark(long long, long long);
	DDBlackMark(void*);
	DDBlackMark(const string&);
	friend class DDBlackManager;
	friend class DDBlackMarkFactory;
	//warning�ı�����
	[[nodiscard]] string printJson(int tab) const;
	[[nodiscard]] string warning() const;
	[[nodiscard]] string getData() const;
	void fill_note();
	//�Ƿ�Ϸ�����
	bool isValid = false;
	bool isClear = false;
	[[nodiscard]] bool isType() const;
	[[nodiscard]] bool isType(const string& strType) const;
	[[nodiscard]] bool isSame(const DDBlackMark&) const;
	[[nodiscard]] bool isSource(long long) const;
    void check_remit();
	DDBlackMark& operator<<(const DDBlackMark&);
	//bool operator<(const DDBlackMark&)const;
};

class DDBlackManager
{
	vector<DDBlackMark> vBlackList;
	//�ƶ˱��ӳ���
	unordered_map<int, unsigned int> mCloud;
	unordered_set<unsigned int> sIDEmpty;
	//���ظ�ӳ���
	multimap<string, unsigned int> mTimeIndex;
	unordered_set<unsigned int> sTimeEmpty;
	unordered_set<unsigned int> sGroupEmpty;
	unordered_set<unsigned int> sQQEmpty;
	//������ָ��ͬ�ļ�¼
	int find(const DDBlackMark&);
	//���¼�¼
	bool insert(DDBlackMark&);
	bool update(DDBlackMark&, unsigned int, int);
	void reset_group_danger(long long);
	void reset_qq_danger(long long);
	bool up_group_danger(long long, DDBlackMark&);
	bool up_qq_danger(long long, DDBlackMark&);
	// bool isActive = false;
public:
	multimap<long long, unsigned int> mGroupIndex;
	multimap<long long, unsigned int> mQQIndex;
	//δע����������Σ�յȼ�
	unordered_map<long long, short> mQQDanger;
	unordered_map<long long, short> mGroupDanger;
	[[nodiscard]] short get_group_danger(long long) const;
	[[nodiscard]] short get_qq_danger(long long) const;
	void isban(FromMsg*);
	string list_group_warning(long long);
	string list_qq_warning(long long);
	string list_self_group_warning(long long);
	string list_self_qq_warning(long long);
	void add_black_group(long long, FromMsg*);
	void add_black_qq(long long, FromMsg*);
	void rm_black_group(long long, FromMsg*);
	void rm_black_qq(long long, FromMsg*);
	void verify(void*, long long);
	void create(DDBlackMark&);
	//��ȡjson��ʽ��������¼
	int loadJson(const std::filesystem::path& fpPath, bool isExtern = false);
	//��ȡ�ɰ汾�������б�
	int loadHistory(const std::filesystem::path& fpLoc);
	void saveJson(const std::filesystem::path& fpPath) const;
};

extern std::unique_ptr<DDBlackManager> blacklist;

class DDBlackMarkFactory
{
	DDBlackMark mark;
	using Factory = DDBlackMarkFactory;
public:
	DDBlackMarkFactory(long long qq, long long group): mark(qq, group)
	{
		mark.comment.clear();
	}

	Factory& type(string strType)
	{
		mark.type = std::move(strType);
		return *this;
	}

	Factory& time(string strTime)
	{
		mark.time = std::move(strTime);
		return *this;
	}

	Factory& note(string strNote)
	{
		mark.note = std::move(strNote);
		return *this;
	}

	Factory& comment(string strNote) 
	{
		mark.comment = strNote;
		return *this;
	}

	Factory& inviterQQ(long long qq)
	{
		mark.inviterQQ = {qq, true};
		return *this;
	}

	Factory& fromQQ(long long qq)
	{
		mark.fromQQ = {qq, true};
		return *this;
	}

	Factory& DiceMaid(long long qq)
	{
		mark.DiceMaid = qq;
		return *this;
	}

	Factory& masterQQ(long long qq)
	{
		mark.masterQQ = qq;
		return *this;
	}

	Factory& sign();

	Factory& upload()
	{
		mark.upload();
		return *this;
	}

	DDBlackMark& product()
	{
		return mark;
	}
};

void AddWarning(const std::string& msg, long long DiceQQ, long long fromGroup);
void warningHandler();
