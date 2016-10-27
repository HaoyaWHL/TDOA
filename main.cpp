#include "DOC.h"
#include "CHAN.h"
#include "DELAY_LOC.h"
#include <stdlib.h>
#include <bitset>
#include <WINSOCK2.H>
#include <string>
#include <iostream>
#include "SOCKET_ADDR.h"
#include "TaylorAlgo.h"	
#pragma comment(lib,"ws2_32.lib")

using namespace std;
int main()
{
	double ReadData();
	int Calculate_ID(char  high, char low);
	double Clculate_char_to_decimal(char one, char two, char three, char four);
	///////////////////////////////////////////////
	//////create the socket
	///////////////////////////////////////////////
	WSADATA wsock;
	SOCKET HostSocket;
	SOCKADDR_IN serAddr;
	int serAddrsize = sizeof(serAddr);


	cout << "initialize the Winsock   ";
	if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
	{
		cout << "falied" << endl;
		return 0;
	}
	else
	{
		cout << "successfully" << endl;

	}


	//create the socket
	cout << "create the Winsock   ";
	if ((HostSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << "failed" << endl;
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "successfullt" << endl;

	}


	//set the ¡®SOCKADDR_IN¡¯ struct address
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(PORT);
	serAddr.sin_addr.s_addr = inet_addr(ADDR);

	cout << "bind the socket and than recv data   ";
	if (bind(HostSocket, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		cout << "failed" << endl;
		closesocket(HostSocket);
		WSACleanup();
		return 0;
	}
	else{

		cout << "successfully." << endl << "Time to recv data:" << endl << endl;
	}
	//finish the step of creating the socket


	/////////////////////////////////////////////////////////////////////////
	//                the second step : recv data                          //
	/////////////////////////////////////////////////////////////////////////
	char buf[20];//because recv data has 19 bytes,and the twentieth is in order to storage the end symbol of buf[]————\0
	memset(buf, 0, sizeof(buf));
	double P1, P2, P3, P4;
	int ID_number;
	double orginData[4];
	////////////////////////////////////////////////////////////////////////////////////////
	while (1)
	{
		recvfrom(HostSocket, buf, sizeof(buf), 0, (SOCKADDR*)&serAddr, &serAddrsize);


		ID_number = Calculate_ID(buf[0], buf[1]);

		P1 = Clculate_char_to_decimal(buf[2], buf[3], buf[4], buf[5]);
		P2 = Clculate_char_to_decimal(buf[6], buf[7], buf[8], buf[9]);
		P3 = Clculate_char_to_decimal(buf[10], buf[11], buf[12], buf[13]);
		P4 = Clculate_char_to_decimal(buf[14], buf[15], buf[16], buf[17]);


		if ((P3 == 0) && (P4==0))
		{
			cout << "the hreatBeat of the base station" << endl;
		}
		else
		{
			
			orginData[0] = P1;
			orginData[1] = P2;
			orginData[2] = P3;
			orginData[3] = P4;
		

		double pos[2] = { 0 };

		XYTDOA xyTDOA;
		xyTDOA.GetData(orginData);
		xyTDOA.displayData();
		xyTDOA.Calculate();//加入对Z计算的代码
		xyTDOA.displayPOS();
		xyTDOA.displayFinal_XY(pos);

		double x = pos[0];
		double y = pos[1];

		TAYLOR_ALGO myTaylor2Dim;
		myTaylor2Dim.ReadTxt(orginData);
		double xyOutput[2];
		myTaylor2Dim.TaylorCalcXY(x, y);//xy
		myTaylor2Dim.GetCalcResult(&(xyOutput[0]), &(xyOutput[1]));
		//cout << xyOutput[0] << "  " << xyOutput[1] << endl;

		double xyz_Initial_Taylor[3] = { 0 };
		xyz_Initial_Taylor[0]=xyOutput[0];
		xyz_Initial_Taylor[1]=xyOutput[1];
		xyz_Initial_Taylor[2]=xyTDOA.Calculate_inital_Z(xyz_Initial_Taylor);
	


		
		TAYLOR_ALGO myTaylor3Dim;
		myTaylor3Dim.ReadTxt(orginData);

		double xyzOutput[3];
		double x_final=xyz_Initial_Taylor[0];
		double y_final=xyz_Initial_Taylor[1];
		double z_final=xyz_Initial_Taylor[2];
		myTaylor3Dim.TaylorCalcXYZ(x_final,y_final,z_final);//xyz
		myTaylor3Dim.GetCalcResult(&(xyzOutput[0]), &(xyzOutput[1]), &(xyzOutput[2]));
		xyzOutput[2] = xyzOutput[2] - 0.15;
		cout<<"the ID number:   "<<ID_number<<"   ";
		cout <<"  X: "<<xyzOutput[0] <<"  Y: "<< xyzOutput[1] <<"  Z: "<< xyzOutput[2] << endl;



		//storage the object location 
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////
	closesocket(HostSocket);
	WSACleanup();


	return 0;
}


int Calculate_ID(char  high, char low)
{
	bitset<8> vec_high(high);
	bitset<8> vec_low(low);

	int count_high = 0;
	int count_low = 0;
	count_high = (vec_high[0] + 2 * vec_high[1] + 4 * vec_high[2] + 8 * vec_high[3]) * 256 + (vec_high[4] + 2 * vec_high[5] + 4 * vec_high[6] + 8 * vec_high[7]) * 4096;

	count_low = (vec_low[0] + 2 * vec_low[1] + 4 * vec_low[2] + 8 * vec_low[3]) + (vec_low[4] + 2 * vec_low[5] + 4 * vec_low[6] + 8 * vec_low[7]) * 16;

	int ID = 0;
	ID = count_high + count_low;

	if (ID == 61166)
	{
		ID = 10;
		return ID;
	}
	ID = (count_high + count_low) % 10;


	return ID;
};


double Clculate_char_to_decimal(char one, char two, char three, char four)
{
	bitset<8> vec1(one);
	bitset<8> vec2(two);
	bitset<8> vec3(three);
	bitset<8> vec4(four);

	double count1 = 0;
	double count2 = 0;
	double count3 = 0;
	double count4 = 0;

	if ((vec1 == '\xEE') && (vec2 == '\xEE') && (vec3 == '\xEE') && (vec4 == '\xEE'))
	{
		return 0;
	}


	count1 = (vec1[0] + 2 * vec1[1] + 4 * vec1[2] + 8 * vec1[3]) * 256 + (vec1[4] + 2 * vec1[5] + 4 * vec1[6] + 8 * vec1[7]) * 4096;
	count2 = (vec2[0] + 2 * vec2[1] + 4 * vec2[2] + 8 * vec2[3]) + (vec2[4] + 2 * vec2[5] + 4 * vec2[6] + 8 * vec2[7]) * 16;

	count3 = (vec3[0] + 2 * vec3[1] + 4 * vec3[2] + 8 * vec3[3]) * 256 + (vec3[4] + 2 * vec3[5] + 4 * vec3[6] + 8 * vec3[7]) * 4096;
	count4 = (vec4[0] + 2 * vec4[1] + 4 * vec4[2] + 8 * vec4[3]) + (vec4[4] + 2 * vec4[5] + 4 * vec4[6] + 8 * vec4[7]) * 16;

	double count = 0;
	count = ((count1 + count2) + (count3 + count4) / 65536) * 250;
	return count;

};