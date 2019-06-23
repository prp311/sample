#include<iostream>

#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"

#include "fbmessage_generated.h"

using namespace FBMessageGen;

flatbuffers::Offset<FBMessageGen::Header> createAndFillHeader(flatbuffers::FlatBufferBuilder &_fbb)
{
    return (FBMessageGen::CreateHeader(_fbb, _fbb.CreateString("client-id-007"))); 
}

flatbuffers::Offset<Register> createAndFillRegister(flatbuffers::FlatBufferBuilder &_fbb)
{
    return (FBMessageGen::CreateRegister(
                _fbb, 
                _fbb.CreateString("client-name-001"),
                _fbb.CreateString("10.56.72.84"),
                4000)); 
}

void createMessageUsingCreateFunction(flatbuffers::FlatBufferBuilder &_fbb, uint8_t **buf, int * size)
{
    auto fbm = FBMessageGen::CreateFBMessage(
            _fbb, 
            createAndFillHeader(_fbb), 
            FBMessageGen::UserMessage_Register, 
            createAndFillRegister(_fbb).Union()); 

    _fbb.Finish(fbm);

    *buf = _fbb.GetBufferPointer();
    *size = _fbb.GetSize();
}

void createMessageUsingBuilder(flatbuffers::FlatBufferBuilder &_fbb, uint8_t **buf, int * size)
{
    auto x = _fbb.CreateString("client-id-007");
    auto y = _fbb.CreateString("client-name-001");
    auto z = _fbb.CreateString("10.56.72.84");
    HeaderBuilder builderhdr_(_fbb);
    builderhdr_.add_senderId(x);
    auto fbHeader = builderhdr_.Finish();

    RegisterBuilder builderma_(_fbb);
    builderma_.add_name(y);
    builderma_.add_ip(z);
    builderma_.add_port(4000);
    auto fbRegister = builderma_.Finish().Union();

    FBMessageBuilder builderfbm_(_fbb);
    builderfbm_.add_userMessage(fbRegister);
    builderfbm_.add_header(fbHeader);
    builderfbm_.add_userMessage_type(UserMessage_Register);
    auto fbMsg = builderfbm_.Finish();
    _fbb.Finish(fbMsg);

    *buf = _fbb.GetBufferPointer();
    *size = _fbb.GetSize();
}

void usage()
{
    std::cout << "\tuse-create   : Use create functions to create the message" <<std::endl;
    std::cout << "\tuse-builder  : Use builder functions to create the message" <<std::endl;
}

int main(int argc, char *argv[])
{
    uint8_t *buf = NULL;
    int     size = 0;

    flatbuffers::FlatBufferBuilder _fbb;

    if(argc != 2)
    {
        usage();
        return 1;
    }

    if(strcmp(argv[1], "use-create") == 0)
    {
        createMessageUsingCreateFunction(_fbb, &buf, &size);
    }
    else if(strcmp(argv[1], "use-builder") == 0)
    {
        createMessageUsingBuilder(_fbb, &buf, &size);
    }
    else
    {
        std::cout << "Invalid Usage" <<std::endl;
        usage();
        return 2;
    }

    const FBMessageGen::FBMessage *msg = FBMessageGen::GetFBMessage(buf);

    std::cout << "Sender-ID : " << msg->header()->senderId()->str() << std::endl;
    std::cout << "          : " << EnumNameUserMessage(msg->userMessage_type()) << " " << msg->userMessage_type() << std::endl;

    if(msg->userMessage_type() == UserMessage_Register)
    {
        Register *usrMsg = (Register *) msg->userMessage();
        std::cout << "Name      : " << usrMsg->name()->str()  << std::endl;
        std::cout << "IP        : " << usrMsg->ip()->str()  << std::endl;
        std::cout << "Port      : " << usrMsg->port() << std::endl;
    }

    return 0;
}
