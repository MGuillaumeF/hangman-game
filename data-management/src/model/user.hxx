#ifndef USER_HXX
#define USER_HXX

#include <string>

#include <odb/core.hxx>

#pragma db object
class user {
public:
  /**
   * @brief Construct a new user object
   * 
   */
  user() {};

  /**
   * @brief Get the Login object
   * 
   * @return const std::string& 
   */
  const std::string& getLogin () const {
    return login_;
  };

  /**
   * @brief Set the Login object
   * 
   * @param login 
   */
  void setLogin(const std::string& login) {
    login_ = login; 
  };

  /**
   * @brief Get the Password object
   * 
   * @return const std::string& 
   */
  const std::string& getPassword () const {
    return password_;
  };

  /**
   * @brief Set the Password object
   * 
   * @param password 
   */
  void setPassword(const std::string& password) {
    password_ = password;
  };

  /**
   * @brief Get the Salt User object
   * 
   * @return const std::string& 
   */
  const std::string& getSaltUser () const {
    return saltUser_;
  };

  /**
   * @brief Set the Salt User object
   * 
   * @param saltUser 
   */
  void setSaltUser(const std::string& saltUser) {
    saltUser_ = saltUser;
  };

  /**
   * @brief Get the Salt Session object
   * 
   * @return const std::string& 
   */
  const std::string& getSaltSession () const {
    return saltSession_;
  };

  /**
   * @brief Set the Salt Session object
   * 
   * @param saltSession 
   */
  void setSaltSession(const std::string& saltSession) {
    saltSession_ = saltSession;
  };

  /**
   * @brief Get the Token object
   * 
   * @return const std::string& 
   */
  const std::string& getToken () const {
    return token_;
  };

  /**
   * @brief Set the Token object
   * 
   * @param token 
   */
  void setToken(const std::string& token) {
    token_ = token;
  };

private:
  friend class odb::access;

  #pragma db id auto
  unsigned long id_;

  std::string login_;
  std::string password_;
  std::string saltUser_;
  std::string saltSession_;
  std::string token_;
};

#pragma db object(user)

#pragma db view object(user)
struct user_stat
{
  #pragma db column("count(" + user::id_ + ")")
  std::size_t count;
};

#endif // USER_HXX