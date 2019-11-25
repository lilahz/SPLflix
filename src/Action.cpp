#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

using namespace std;

BaseAction::BaseAction() {
    status= PENDING;
}

//============================================Action Class Methods======================================================

/*
 * Returns the status of the current action
 * @return ActionStatus PENDING, COMPLETED, ERROR based on the status.
 * (default)
 */
ActionStatus BaseAction::getStatus() const {
    return status;
}

/*
 * Updates the status of the action to COMPLETED.
 * (default)
 */
void BaseAction::complete() {
    status = COMPLETED;
}

/*
 * Updates the status of the action to ERROR and update the error massage.
 * (default)
 */
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

/*
 * Return string of the relevant error massage.
 * (default)
 */
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

std::string BaseAction::getStatusString(ActionStatus status) const {
    if (status == PENDING){
        return "PENDING";
    }
    else if (status == COMPLETED) {
        return "COMPLETED";
    }
    else if (status == ERROR) {
        return "ERROR";
    }
    else return nullptr;
}

//=================================================Create User==========================================================
// TODO: think if we need to keep the constructors.
//CreateUser::CreateUser() {}

/*
 * Start the createUser action. Create new user based on the input algorithm,
 * and add the new user to the userMap.
 * @param: Session &sess: reference to Session object.
 * (default)
 */
void CreateUser::act(Session &sess) {
    User* user;
    std::string userName = sess.getUserName();
    std::string userAlgo = sess.getThirdParameter();
    if ((userAlgo != "len") & (userAlgo != "rer") & (userAlgo != "gen")) {
        // actionsLog.push_back()
        error("Invalid algorithm input.");
        std::cout<< "Error: " << getErrorMsg() << endl;
    }
    else if (sess.findInUserMap(userName) != nullptr) {
       error("User already exist.");
        std::cout<< "Error: " << getErrorMsg() << endl;
    }
    else if (userAlgo == "len") {
        user = new LengthRecommenderUser(userName);
        sess.addToUserMap(userName , user);
        complete();
    }
    else if (userAlgo == "rer") {
        user = new RerunRecommenderUser(userName);
        sess.addToUserMap(userName , user);
        complete();
    }
    else if (userAlgo == "gen") {
        user = new GenreRecommenderUser(userName);
        sess.addToUserMap(userName , user);
        complete();
    }
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string CreateUser::toString() const {
    std::string str;
    str = "CreateUser " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *CreateUser::clone() const {
    return new CreateUser(*this);
}

//==============================================Change Active User======================================================

/*
 * Start the ChangeActiveUser action. Change the active user to the input user.
 * (default)
 */
void ChangeActiveUser::act(Session &sess) {
    std::string userName;
    userName = sess.getUserName();

    if (sess.findInUserMap(userName) == nullptr) {
        error("user does not exist.");
        cout << "ERROR: " << getErrorMsg() << endl;
    }
    else {
        sess.setActiveUser(sess.findInUserMap(userName));
        complete();
    }
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string ChangeActiveUser::toString() const {
    std::string str;
    str = "ChangeActiveUser " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *ChangeActiveUser::clone() const {
    return new ChangeActiveUser(*this);
}
//=================================================Delete User==========================================================

/*
 * Start the DeleteUser action.
 * Deleted the user from the userMap.
 * (default)
 */
void DeleteUser::act(Session &sess) {
    std::string userName;
    userName = sess.getUserName();

    if (sess.findInUserMap(userName) == nullptr) {
        error("User does not exist.");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else {
        sess.deleteFromUserMap(userName);
        complete();
    }
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string DeleteUser::toString() const {
    std::string str;
    str = "DeleteUser " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *DeleteUser::clone() const {
    return new DeleteUser(*this);
}


//================================================Duplicate User========================================================

/*
 * Start the DuplicateUser action.
 * Creates new user with new given name, but with same algorithm and watching history
 * of exist user.
 * (default)
 */
void DuplicateUser::act(Session &sess) {
    std::string userName, newUserName;
    userName = sess.getUserName();
    newUserName = sess.getThirdParameter();

    if (sess.findInUserMap(userName) == nullptr) {
        error("User does not exist.");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else if (sess.findInUserMap(newUserName) != nullptr) {
        error("User name already taken");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else {
        sess.addToUserMap(newUserName, sess.findInUserMap(userName)->duplicateUser(newUserName));
        complete();
    }
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string DuplicateUser::toString() const {
    std::string str;
    str = "DuplicateUser " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *DuplicateUser::clone() const {
    return new DuplicateUser(*this);
}

//=============================================Print Content List=======================================================

/*
 * Start the PrintContentList action.
 * Prints all the available content to the active user.
 * (default)
 */
void PrintContentList::act(Session &sess) {
    for (int i=0; i<sess.getContent().size(); i++) {
        Watchable *print = sess.getContent().at(i);
        std::cout << print->getId()+1<< ". " << print->toString()<< " " << print->getLength()
        << " minutes" << " [" << print->tagsToString()+"]" << std::endl;
    }
    complete();
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string PrintContentList::toString() const {
    std::string str;
    str = "PrintContentList " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *PrintContentList::clone() const {
    return new PrintContentList(*this);
}

//=============================================Print Watch History======================================================

/*
 * Start the PrintWatchHistory action.
 * Prints the watch history of the active user.
 * (default)
 */
void PrintWatchHistory::act(Session &sess) {
    std::cout << "Watch history for " << sess.getActiveUser()->getName() << std::endl;
    for (int i=0; i<sess.getActiveUser()->getHistory().size();i++){
       std::cout << i+1 <<". " << sess.getActiveUser()->getHistory().at(i)->toString() << std::endl;
    }
    complete();
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string PrintWatchHistory::toString() const {
    std::string str;
    str = "PrintWatchHistory " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *PrintWatchHistory::clone() const {
    return new PrintWatchHistory(*this);
}

//=====================================================Watch============================================================
/*
 * Start the Watch action.
 * Watching content (movie/episode) based on given content id or reccomendation.
 * (default)
 */
void Watch::act(Session &sess) {
    std::string answer="y";
    std::string stringid;
    std::cin >> stringid ;
    int intid = std::stoi(stringid) - 1 ;
    Watchable* watched = sess.getContent().at(intid);
    while (answer=="y") {
        std::cout << "Watching "<< watched->toString()<< std::endl ;
        sess.getActiveUser()->addToHistory(watched);
        sess.getActiveUser()->updateRec(watched);
        watched = watched->getNextWatchable(sess);
        std::cout << "We recommend watching " ;
        std::cout << watched->toString();
        std::cout << ", continue watching? [y/n]" << std::endl;
        std::cin >>  answer;
        complete();
        sess.addToActionsLog(this);
    }
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string Watch::toString() const {
    std::string str;
    str = "Watch " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *Watch::clone() const {
    return new Watch(*this);
}

//==============================================Print Actions Log=======================================================
/*
 * Start the PrintActionLog action.
 * Prints all the actions that were performed in the current session with their statuses and messages.
 * (default)
 */
void PrintActionsLog::act(Session &sess) {
    for (int i = sess.getActionLog().size() - 1; i >= 0; i--) {
        cout << sess.getActionLog().at(i)->toString() << endl;
    }
    complete();
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string PrintActionsLog::toString() const {
    std::string str;
    str = "PrintActionLog " + getStatusString(getStatus());
    if (getStatus() == ERROR){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

BaseAction *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

//=====================================================Exit=============================================================
/*
 * Start the Exit action.
 */
void Exit::act(Session &sess) {
    complete();
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string Exit::toString() const {
    std::string str;
    str = "Exit " + getStatusString(getStatus());
    return str;
}

BaseAction *Exit::clone() const {
    return new Exit(*this);
}
