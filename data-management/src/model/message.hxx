
/**
 * @brief message.hxx DO NOT MODIFY THIS FILE, this file is a generated model
 * class
 */

#ifndef __GENERATED_MODEL_OBJECT_MESSAGE_HXX__
#define __GENERATED_MODEL_OBJECT_MESSAGE_HXX__

#include "./chat.hxx"
#include "./root_model_object.hxx"
#include "./user.hxx"

#include <ctime>
#include <memory>
#include <string>

class user;
class chat;

/**
 * @brief class of message object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class message final : public root_model_object {
private:
  friend class odb::access;
  std::string m_content;
#pragma db not_null
  std::shared_ptr<user> m_sender;
  std::time_t m_send_date;
#pragma db not_null
  std::shared_ptr<chat> m_chat;

public:
  /**
   * @brief Construct a new message object
   *
   */
  message() = default;

  /**
   * @brief Set the content of object
   *
   * @param content The content of object
   */
  void setContent(const std::string &content) { m_content = content; };

  /**
   * @brief Get the content of object
   *
   * @return const std::string& the content of object
   */
  const std::string &getContent() const { return m_content; };

  /**
   * @brief Set the sender of object
   *
   * @param sender The sender of object
   */
  void setSender(const std::shared_ptr<user> &sender) { m_sender = sender; };

  /**
   * @brief Get the sender of object
   *
   * @return const std::shared_ptr<user>& the sender of object
   */
  const std::shared_ptr<user> &getSender() const { return m_sender; };

  /**
   * @brief Set the send_date of object
   *
   * @param send_date The send_date of object
   */
  void setSendDate(const std::time_t &send_date) { m_send_date = send_date; };

  /**
   * @brief Get the send_date of object
   *
   * @return const std::time_t& the send_date of object
   */
  const std::time_t &getSendDate() const { return m_send_date; };

  /**
   * @brief Set the chat of object
   *
   * @param chat The chat of object
   */
  void setChat(const std::shared_ptr<chat> &chat) { m_chat = chat; };

  /**
   * @brief Get the chat of object
   *
   * @return const std::shared_ptr<chat>& the chat of object
   */
  const std::shared_ptr<chat> &getChat() const { return m_chat; };

  /**
   * @brief method to extract object from property tree
   *
   * @return The message found
   */
  static std::unique_ptr<message>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<message> parsedObject =
        root_model_object::parse<message>(property_tree);
    const boost::optional<std::string> content =
        property_tree.get_optional<std::string>("content");
    if (content) {
      parsedObject->setContent(*content);
    }

    const boost::optional<std::time_t> send_date =
        property_tree.get_optional<std::time_t>("send_date");
    if (send_date) {
      parsedObject->setSendDate(*send_date);
    }

    return parsedObject;
  }

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error vector of validation
   */
  std::vector<model_error> getErrors() const {
    std::vector<model_error> errors;
    // TODO add implementation
    return errors;
  }

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "message"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "messages"; }
};

#pragma db object(message)

#pragma db view object(message)
struct message_stat {
#pragma db column("count(" + message::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_MESSAGE_HXX__

#ifdef ODB_COMPILER

#endif
