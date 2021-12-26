#ifndef USER_HXX
#define USER_HXX

#include <string>

#include <odb/core.hxx>

#pragma db object
class user {
public:
  user();

  const std::string& getLogin () const;
  void setLogin(const std::string& login);

  const std::string& getPassword () const;
  void setPassword(const std::string& password);

  const std::string& getSaltUser () const;
  void setSaltUser(const std::string& saltUser);

  const std::string& getSaltSession () const;
  void setSaltSession(const std::string& saltSession);

  const std::string& getToken () const;
  void setToken(const std::string& saltToken);

private:
  friend class odb::access;

  #pragma db id auto
  std::uint64_t id_;

  std::string login_;
  std::string password_;
  std::string saltUser_;
  std::string saltSession_;
  std::string token_;
};

#pragma db object(user)
#pragma db ember(user:login_) id

#pragma db view object(user)
struct user_stat
{
  #pragma db column("count(" + user::id_ + ")")
  std::size_t count;
};

#endif // USER_HXX