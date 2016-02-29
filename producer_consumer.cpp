// producer_consumer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>

using namespace std;



class MyQueque 
{
	std::queue<string> messages;
	boost::mutex lock;
	boost::atomic<int> size;
	

public:
	bool isClearable = false;
	void push(string &c)
	{
		boost::lock_guard<boost::mutex> grd(lock);
		messages.push(c);
		++size;
	}
	string  pop()
	{
		string  result = "empty string";
		if (--size >= 0)
		{
			boost::lock_guard<boost::mutex> grd(lock);
			result = messages.front();
			messages.pop();
		}
		else
		{
			++size;
		}

		return result;
	}
	int getsize()
	{
		return size;
	}

};

MyQueque myqueue;

void producer()
{
	string m;
	for (int i = 0; i <= 10000; ++i)
	{
		m = std::to_string(i); // = static_cast<Message>(i);
		myqueue.push(m);
		std::cout << "push " << m << endl;
	//	if (i == 10000) myqueue.isClearable = true;
	}
	myqueue.isClearable = true;
	return;
}

void consumer()

{
	string m;
	while(true)
	{
		if (myqueue.getsize() > 0)
		{

			m = myqueue.pop();
			//char *c = static_cast<char*>(m);
			std::cout << "pop " << m << endl;
		}
		else {
			std::cout << "QUEUE IS EMTY" << endl;
			if (myqueue.isClearable) return;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	boost::thread producerThread(producer);
	boost::thread consumerThread(consumer);

	producerThread.join();
	consumerThread.join();
	return 0;
}

