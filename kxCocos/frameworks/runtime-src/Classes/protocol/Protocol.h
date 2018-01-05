/*
* ͨѶЭ�� ��Ҫ����������Ͱ�ͷ
*
*/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum NetMainCMD
{
    CMD_LOGIN = 1,      // ��¼
    CMD_USER,		    // �û�����
};

inline int MakeCommand(int main, int sub)
{
    return (main << 16) | (sub & 0x0000ffff);
}

struct Head
{
    int length;
    int cmd;
    int id;

    inline void MakeCommand(int main, int sub)
    {
        cmd = (main << 16) | (sub & 0x0000ffff);
    }

    inline int SubCommand()
    {
        return cmd & 0x0000ffff;
    }

    inline int MainCommand()
    {
        return cmd >> 16;
    }

    inline void* data()
    {
        return this + 1;
    }
};

#endif
