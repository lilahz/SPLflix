#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/User.h"

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
    std::string getStatusString(ActionStatus status) const;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;

};

class CreateUser  : public BaseAction {
public:
    //CreateUser(const std::string &name, const std::string &algo);
    CreateUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;
private:
    std::string newUserName;
    std::string algo;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;

private:
    User* activeUser;
};

class DeleteUser : public BaseAction {
public:
    //DeleteUser(User* user);
    DeleteUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
private:
    User* userToDelete;
};


class DuplicateUser : public BaseAction {
public:
    //DuplicateUser(User* user, std::string newName);
    DuplicateUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;

private:
    User* userToDuplicate;
    std::string newName;
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
	virtual void act (Session& sess);
	virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
	virtual void act (Session& sess);
	virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
    Watch();
	virtual void act(Session& sess);
	virtual std::string toString() const;

private:


};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};
#endif
