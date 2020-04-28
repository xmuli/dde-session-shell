#ifndef LOCKWORKER_H
#define LOCKWORKER_H

#include "src/global_util/dbus/dbuslockservice.h"
#include "src/global_util/dbus/dbuslogin1manager.h"
#include "src/global_util/dbus/dbushotzone.h"
#include "src/session-widgets/userinfo.h"
#include "../libdde-auth/interface/deepinauthinterface.h"
#include "../libdde-auth/deepinauthframework.h"
#include "src/session-widgets/authinterface.h"

#include <QObject>
#include <QWidget>

#include <com_deepin_daemon_logined.h>
#include <com_deepin_daemon_accounts.h>
#include <com_deepin_sessionmanager.h>

using LoginedInter = com::deepin::daemon::Logined;
using AccountsInter = com::deepin::daemon::Accounts;
using SessionManager = com::deepin::SessionManager;

class SessionBaseModel;
class LockWorker : public Auth::AuthInterface, public DeepinAuthInterface
{
    Q_OBJECT
public:
    explicit LockWorker(SessionBaseModel *const model, QObject *parent = nullptr);

    void switchToUser(std::shared_ptr<User> user) override;
    void authUser(const QString &password) override;

    void enableZoneDetected(bool disable);

    void onDisplayErrorMsg(AuthAgent::Type type, const QString &errtype, const QString &msg) override;
    void onDisplayTextInfo(AuthAgent::Type type, const QString &msg) override;
    void onPasswordResult(AuthAgent::Type type, const QString &msg) override;

private:
    void onUserAdded(const QString &user) override;
    void saveNumlockStatus(std::shared_ptr<User> user, const bool &on);
    void recoveryUserKBState(std::shared_ptr<User> user);
    void authStatusChanged(bool status);

    // lock
    void lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message);
    void onUnlockFinished(bool unlocked);

    void userAuthForLock(std::shared_ptr<User> user);
    void checkDBusServer(bool isvalid);
private:
    bool m_authenticating;
    bool m_isThumbAuth;
    DBusLockService *m_lockInter;
    DBusHotzone *m_hotZoneInter;
    DeepinAuthFramework *m_authFramework;
    QString m_password;
    QMap<std::shared_ptr<User>, bool> m_lockUser;
    SessionManager *m_sessionManager;
};

#endif // LOCKWORKER_H
