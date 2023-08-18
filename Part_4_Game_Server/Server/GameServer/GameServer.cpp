﻿#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Job.h"
#include "Room.h"
#include "Player.h"
#include "DBConnectionPool.h"
#include "DBBind.h"

enum
{
	WORKER_TICK = 64,
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 18 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDB;Trusted_Connection=Yes;"));

	//Create Table
	{
		auto query = L"									\
			DROP TABLE IF EXISTS[dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY, \
				[gold] INT NULL,						\
				[name] NVARCHAR(50) NULL,				\
				[createDate] DATETIME NULL				\
			);";

		DBConnection* dbCon = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbCon->Execute(query));
		
		GDBConnectionPool->Push(dbCon);
	}

	// Add Data
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbCon = GDBConnectionPool->Pop();

		DBBind<3, 0> dbBind(*dbCon, L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)");
		
		int32 gold = 100;
		dbBind.BindParam(0, gold);

		WCHAR name[100] = L"천성욱";
		dbBind.BindParam(1, name);

		TIMESTAMP_STRUCT ts = { 2023, 8, 16 };
		dbBind.BindParam(2, ts);

		ASSERT_CRASH(dbBind.Execute());

		//
		//// 기존에 바인딩 된 정보 날림
		//dbCon->Unbind();

		//// 넘길 인자 바인딩
		//int32 gold = 100;
		//SQLLEN len = 0;

		//WCHAR name[100] = L"천성욱";
		//SQLLEN nameLen = 0;
		//
		//TIMESTAMP_STRUCT ts = {};
		//ts.year = 2023;
		//ts.month = 8;
		//ts.day = 16;
		//SQLLEN tsLen = 0;

		//ASSERT_CRASH(dbCon->BindParam(1, &gold, &len));
		//ASSERT_CRASH(dbCon->BindParam(2, name, &nameLen));
		//ASSERT_CRASH(dbCon->BindParam(3, &ts, &tsLen));

		//ASSERT_CRASH(dbCon->Execute(L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)"));
		//

		GDBConnectionPool->Push(dbCon);
	}

	// Read
	{
		DBConnection* dbCon = GDBConnectionPool->Pop();
		
		DBBind<1, 4> dbBind(*dbCon, L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)");

		int32 gold = 100;
		dbBind.BindParam(0, gold);

		int32 outId = 0;
		int32 outGold = 0;
		WCHAR outName[100];
		TIMESTAMP_STRUCT outDate = {};

		dbBind.BindCol(0, OUT outId);
		dbBind.BindCol(1, OUT outGold);
		dbBind.BindCol(2, OUT outName);
		dbBind.BindCol(3, OUT outDate);

		ASSERT_CRASH(dbBind.Execute());
		//
		//dbCon->Unbind();

		//// 넘길 인자 바인딩
		//int32 gold = 100;
		//SQLLEN len = 0;

		//ASSERT_CRASH(dbCon->BindParam(1, &gold, &len));

		//int32 outId = 0;
		//SQLLEN outIdLen = 0;
		//ASSERT_CRASH(dbCon->BindCol(1, &outId, &outIdLen));

		//int32 outGold = 0;
		//SQLLEN outGoldLen = 0;
		//ASSERT_CRASH(dbCon->BindCol(2, &outGold, &outGoldLen));

		//WCHAR outName[100];
		//SQLLEN outNameLen = 0;
		//ASSERT_CRASH(dbCon->BindCol(3, outName, len32(outName), &outNameLen));

		//TIMESTAMP_STRUCT outDate = {};
		//SQLLEN outDateLen = 0;
		//ASSERT_CRASH(dbCon->BindCol(4, &outDate, &outDateLen));

		//ASSERT_CRASH(dbCon->Execute(L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)"));

		
		wcout.imbue(locale("kor"));
		while (dbCon->Fetch())
		{
			wcout << "id: " << outId << " Gold: " << outGold << " Name: " << outName << endl;
			wcout << "Date: " << outDate.year << "/" << outDate.month << "/" << outDate.day << endl;
		}

		GDBConnectionPool->Push(dbCon);
	}

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		 MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				while (true)
				{
					DoWorkerJob(service);
				}
			});
	}

	// Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}