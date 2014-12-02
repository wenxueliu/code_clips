


注：

前32个为保留进程
每个进程有一个 _Stack(lua_newstate)
所有的操作都主要围绕 _Stack 操作

Process
===============================
NodeMap : vector<*Process> 记录 Process->_Id : Process
NodeId : 节点号
MsgIdGen : 发送的消息总个数

process::process(int id)
-----------------------------
参数： id: 进程id
操作
* NodeId : 记录全部 ID 号
* _Id : 当前进程的 id
* 开始计时
* 调用 initNode()

返回 ： 无

Process::~Process(void)
参数：无
操作：
* lua_close(_Stack) 回收
返回：无


void Process::senMsg(const LN_MSG_TYPE& msg)
-----------------------------
参数：msg 要发送的消息

操作
* 如果消息不为空，_Channel.Put()
* 如果消息为空

	head = msg[0]
	BuildMessageReturnValues(head);
	Runtime::GetInstance().GetSchedule().PutTask(*this);

返回： 无

LN_MSG_TYPE Process::RecvMsg()
-----------------------------
参数： 无

操作
* string msg = _Channel.TryGet()

返回：msg

Process::CreateNode()
-----------------------------
参数： 无

操作： 调用 CreateNode(0)

返回： NodeId

Process::CreateNode(int)
-----------------------------
参数： int id ： id 号

操作： 
* 校验 id 有效性 current < id < NodeMap.size() RESERVED_PID < id < current
* node = new Process(id)

返回： node->_Id

int Process::Spawn(lua_State *l)
-----------------------------
参考 int GLR::Process::SpawnEx( lua_State *l )


Process::GetNodeById(LN_ID_TYPE id)
c
参数： id
操作： 检查 id 在 NodeMap 内，且 NodeMap[i] 不为 NULL
返回： NodeMap[id]

int GLR::Process::GetOption(lua_State *l) 
参数: 略

操作：
* Galaxy::GalaxyRT::CProcess *_pCProcess = GLR::Runtime::_pCProcess
* 将 _pCProcess->Arguments中的每个元素组成的 table，压入栈（l）顶

返回：1

int GLR::Process::SetOptions(lua_State *l)
-----------------------------
参数：略

操作：
* Galaxy::GalaxyRT::CProcess *_pCProcess = GLR::Runtime::_pCProcess
* _pCProcess->SetArgument(str)

返回：1

void Process::InitNode(void)
-----------------------------
参数：无

操作：
* lua 与 c 交互的常规做法
* 初始化 Process
返回：无

int Process::SendMsgToNode(lua_State *l)
-----------------------------
参数：略

操作：
* 初始化 pack_msg
* SendMsgToNode(id, pack_msg)

返回：1

注
_Stack

{"msgid":msgid, "corrid": corrid, "Stamp":head->_Protocol._Stamp}


void Process::Preempt(lua_State *l, lua_Debug *ar) 
-----------------------------



void Process::StackDump()
-----------------------------



void Process::MoveValue(lua_State *src, lua_State *dst, int index)
-----------------------------
参数：
* src ： 源stack
* dst ： 目标 stack
* index ： 源 stack 索引

操作：
* 根据 src[index] 的类型，将其压入栈 dst。
* 遇到 table，调用 MoveTable()

返回：无



void Process::MoveTable(lua_State *src, lua_State *dst, int index)
-----------------------------
参数：
* src ： 源stack
* dst ： 目标 stack
* index ： 源 stack 索引


操作：

* 遍历 table，将 table 中的每一个key 或 value调用 MoveValue,最后将其组成的 table 压入 dst

返回：无

void Process::SetArgumentsForSpawnedProcess(lua_State *l, Process &node, int begin_index)
-----------------------------
参数
* l : 源 stack
* node : 目标stack
* begin_index : 索引起始位置

操作：
* 将 l 中 从 begin_index 开始的元素压入 node 中

返回：无


void Process::BuildMessageReturnValues(GLRPROTOCOL *head)
-----------------------------
参数：GLRPROTOCOL 协议结构体

操作：操作栈 _Stack，其新增情况见注

返回：无

注：

Stack

{ stamp : _Protocol._Stamp, msgid: _Msg._MsgId, :corrid: _Msg._CorrId }
{ "host" : _Host._V2._Host, "port": "_Host._V2._Port",  "gpid" : _Route._FromGpid, "attr" : { "stamp" : _Protocol._Stamp, "msgid": _Msg._MsgId, "corrid": _Msg._CorrId } }
{ addr : {host: host, port:port,  gpid:gpid} } 
 _Protocol._Type
 
 
lua 中 global 到底扮演着怎样的角色？其作用域如何, 栈变化？

Process::ProcessStatus::STATE Process::State() const
-----------------------------
参数：无
操作：返回进程状态，有CREATED,STOPED,RUNNING,YIELD,SUSPEND,RECV_WAIT,RECV_RETURN,INT_WAIT,INT_RESP,GLR_CALL,KILLED。
返回：ProcessStatus::STATE


int Process::Yield()
-----------------------------
参数：无
操作：ret = lua_yield(_Stack, 0)
返回：ret


int Process::Debug(lua_State *l)
-----------------------------
参数：无
操作：luaL_loadfile(l, NULL);
返回：0

int Process::Interrupt(lua_State *l)
-----------------------------
参数：略
操作：
* 获取进程号，调用 GetNodeById()
* 修改 process 状态
* 调用 Runtime::GetInstance().GetBus().Interrupt(device, n._Status._Tick, l);
* 如果 n._Status._State == ProcessStatus::INT_RESP， 返回 n._Status._NArg
* 否则调用 ret = n.Yield()

返回：略

void Process::SendExitMsg()
-----------------------------
参数：无
操作：
* 调用 SendMsgToNode(_ParentId, buffer, GLRPROTOCOL::EXIT);
返回：无

void Process::Destory(LN_ID_TYPE pid)
-----------------------------
参数：pid 进程id
操作：
* 通过 NodeMap 获取进程 p
* 如果 p == NULL 返回
* 如果 p._Status._State 为 RECV_WAIT,INT_WAIT,STOPED,KILLED， 调用 
		p->SendExitMsg(), 
		delete p
		NodeCount--
		if NodeCount == 0  调用 Runtime::GetInstance().ElegantExit()
* 否则 p->_Status._Killed = true

返回：无

void Process::SendMsgToNode(LN_ID_TYPE pid, const std::string& msg, GLRPROTOCOL::MSG_TYPE /*type*/)
-----------------------------
参数
* pid
* msg
* MSG_TYPE
操作：
* ....
* 调用 GetNodeById(pid).SendMsg(msg);
* ....

返回：无

int Process::Status(lua_State *l)
-----------------------------
参数：略
操作：
* 从栈底获得 pid
* nd = GetNodeById(pid)
* Stack

  {"state":nd._Status._State, "start_stamp":nd._Status._Start, "last_op_stamp":nd._Status._LastOp,"sche_tick":nd._Status._Tick,"msg_num":nd._Channel.Size()}
  true

返回：2



int Process::AllProcesses(lua_State *l)
-----------------------------
参数：略
操作：
* Stack(l)
    {0:NodeMap[0], 1:NodeMap[1]....}
    true
返回：2

int GLR::Process::GetNodeAddr(lua_State *l)
-----------------------------
参数：略
操作：
* Stack(l)
	Runtime::GetInstance().NodeId()
    Runtime::GetInstance().Host()
  
返回：2

int GLR::Process::GetFilePath(lua_State *l)
-----------------------------
参数：略
操作：
* Stack(l)
     Process::GetNodeBy(id)._Path

返回：1

int GLR::Process::Kill(lua_State *l)
-----------------------------
参数：略
操作：
* 从栈 l 得到栈底元素 gpid
* 调用 Destory(gpid)

返回：待定

Process::SpawnEx( lua_State *l )
-----------------------------
参数： lua_State *
操作： 
* 检查参数的有效性
* CreateNode(int)
* 根据条件调用 node.Entry(module, method) OR node.EntryGar(GLR::Runtime::_GarFile, module, method)
* 设置 node._ParentId
* SetArgumentsForSpawnedProcess(l,node, 4)
* Schedule::GetInstance(id, true)
返回： 
关系： GetNodeById
注：l 栈顶到底依次是 bindPid, module, method




void GLR::Process::EntryGar(const std::string& Gar, const std::string& module, const std::string& entry, ...)
参数

* Gar
* module ：模块名
* entry : 

操作

* 调用 xxxx(Gar.c_str())
* Entry(module, entry)

返回值

	空
	
注

_Stack
          Gar.c_str()
		  xxxxx
	glr   {"__gar__": Gar.c_str(), "run_gar":xxxx}


void GLR::Process::Entry(const std::string& module, const std::string& entry, ...)
参数
module : 模块名
entry : 以"."分隔的 string。如A.B.C.D[:E] ([]可选)

操作

* require(module)
* 获取 module内的各个属性（entry中以“.”分隔），后压栈
* 检查栈

返回值 ： 无

注

各个属性的长度不能超过 1024
require(module) 返回值只必须是 userdata 或 table

_Stack
  	"module"
  	requre
    Gar.c_str()
	xxxxx
	{"__gar__": Gar.c_str(), "run_gar":xxxx}
	--------------------
  	requre(module)
    Gar.c_str()
	xxxxx
	{"__gar__": Gar.c_str(), "run_gar":xxxx}
	--------------------
	result["D"]
	result["E"]
	result["D"]
	result["C"]
	result["B"]
	result["A"]
	result = requre(module)
    Gar.c_str()
	xxxxx
	{"__gar__": Gar.c_str(), "run_gar":xxxx}
	
void GLR::Process::SetTimeOut(int timeout)
-----------------------------
参数： timeout
操作
* 调用 Runtime::GetInstance().GetBus().Interrupt(0, self._Status._Tick, self._Stack)

返回： 无
 
 
 
#问题

\#pragma once 与 \#ifndef 是否只要其中一个就可以？

##LuaNode.cpp 
-----------------------------
	17 int32_t Process::NodeId;  为什么没有初始值
	
	635     |   index=src_top+index+1;   如果 index = -10, src_top = 2, index如果无效，应该早抛出异常，而且异常类型似乎有问题。不便于定位问题。
	


	为什么？
		1077     |   if (current == 0)
		1078     |   {
		1079     |   |   NodeId += RESERVED_PID+1;
		1080     |   |   current += RESERVED_PID+1;
		1081     |   }

	1158        if (module == NULL)     此处应该为 method
 	115         if (module == NULL)     此处应该为 method

	999     char tmp[1024] = { };   很奇怪，可以用 string 么，不会有字符串溢出的可能性，要不可以检查调用 copy() 时字符长度。

    入口参数 lua_State* l 需要检验 NULL == l 么？



##Runtime/Object.hpp

###1 
 60     | CContainer::~CContainer()
 61     | {
 62     |   |for(std::list<CObject*>::iterator it = _list.begin();it!=_list.end();++it)
 63     |   |{
 64             delete (*it);
 65     |   |}
 66     | }

###2 
65后面是否需要加  it.clear() ？

145     | void CNursery::Realse()
146     | {
147     |    _List.clear();
148     | }

 80     | bool CContainer::Clear()
 81     | {
 82          _list.clear();
 83     |   |return true;
 84     | }


list 内是指针成员，如果直接调用Release, 会是否指针所指的内存么？

###3
187     | CByteArray::CByteArray(const void *_data,unsigned int _Length)
188     |   |:_Buffer(NULL)
189     | {
190     |   |if(_data==NULL)
191     |   |{
192             THROW_EXCEPTION_EX("_data is null");
193             return;
194     |   |}
195 
196     |   |Allocate(_Length+1);
197     |   |memcpy(_Get(),_data,_Length);
198     |   |Length(_Length);
199       }

memcpy(_Get(),_data,_Length); 中如果 _Length 的长度大于 strlen(_data) 会发生什么？


CRT_memset()
CRT_snprintf()
CRT_len()




