//�����⣺����ó�������ָ����ַ
/*
#include <stdio.h>
typedef void(*voidFuncPtr)(void);

void test2(void)
{
	voidFuncPtr P1 =NULL;
	P1 = (voidFuncPtr)(0x10000);
	P1();
}

void main()
{
	test2();
}
*/


/*
#include <stdio.h>

void main()
{
	int a = 10;
	int b = 0;
	//const int* p0 = &a; //const���ε���*p0��ֵ���ܱ䣬ָ��ĵ�ַ���Ա�
	//int const* p1 =&a;//const���ε���*p1��ֵ���ܱ䣬ָ��ĵ�ַ���Ա�
	int* const p2 = &a;//const���ε���p2(ָ��)��ָ��ĵ�ַ���ܱ�
	//p2 = &b;
	//const int* const p3 = &a;//const�����ε��ˣ�ָ���ֵ�͵�ַ�����ܱ�
	//*p3 =20;
	printf("%d\n",*p2);

}
*/

//�����⣺���ģʽ��С��ģʽ������
//С�ˣ��͵�ַ��ŵ�λ�ֽ�
//��ˣ��ߵ�ַ��Ÿ��ֽ�
#include <stdio.h>
void main()
{
	unsigned char* ptr = NULL;
	unsigned int a = 0x1234;
	ptr = (unsigned char*)&a;
	printf("%x\n",*ptr);//�ص�ַ��ŵ��ֽ�
	printf("%x\n",*(ptr+1));//�ߵ�ַ��Ÿ��ֽ�
}