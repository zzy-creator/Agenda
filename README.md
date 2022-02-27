# 项目内容

本项目实现了一个基于命令行的议程管理系统。系统需提供用户登录、新用户注册，已注册用户登陆后用户可以注销(delete)当前用户账户，查询(query)用户名单，也可以添加(add)、删除(delete)、查询(query)系统中记录的会议安排等管理功能。



# 实现框架

本项目采用**三层架构**

![](/img/1.png)

Entity层：Date、Meeting、User

DAL层：Storage

BLL层：AgendaService

UI层：AgendaUI



程序功能请参考 *Agenda需求文档.pdf*

新拓展的功能为：

发起者可以为其发起的会议设置一名会议管理员（会议管理员可以为空）

若某用户是会议管理员（该用户必须也参与了该会议且不为发起者），则该用户可以增添或者删除参与者



# 程序运行

运行 *AgendaUI.cpp* 进行使用，数据使用的是`/tmp/meetings.csv`和`/tmp/users.csv`，如需使用别的数据，请修改 *Path.h*

UI界面具体命令对应功能请参考 *AgendaUI基础约束.pdf*



# 运行样例

1.注册一个新用户，username为”z“，password为”1“，email为”2“，phone为”3“

![](/img/2.png)

2.登录用户”z“，密码为”1“

![](/img/3.png)

3.登录用户”z“后，成功创建一个会议

![](/img/4.png)
