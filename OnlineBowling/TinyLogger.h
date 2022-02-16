#pragma once
class TinyLogger
{
public:
	TinyLogger(string inName, bool inPriority = false)
		: mPriority(inPriority)
	{
		mName = inName;
		if (mPriority)
		{
			LOG("###############################################", NULL);
		}
		else
		{
			LOG("-----------------------------------------------", NULL);
		}
		LOG((mName + "---Start").c_str(), NULL);
	}
	~TinyLogger()
	{
		LOG((mName + "---End").c_str(), NULL);
		if (mPriority)
		{
			LOG("###############################################", NULL);
		}
		else
		{
			LOG("-----------------------------------------------", NULL);
		}
	}

	void SetPriority(bool inPriority) { mPriority = inPriority; }

private:
	string mName;
	bool mPriority;
};

