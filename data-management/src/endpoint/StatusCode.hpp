#ifndef __STATUS_CODE_HPP__
#define __STATUS_CODE_HPP__

/**
 * @brief all status code
 */
enum class status_code {
 /**
  * status when request processing success
  */
  OK = 200,
  /**
   * status when request content has bad format or content
   */
  BAD_REQUEST = 400,
  /**
   * status when request not found a specific endpoint
   * example : object type not found for CRUD or custom endpoint not found
   */
  NOT_FOUND = 404,
  /**
   * status when request of type CRUD has bad method 
   */
  METHOD_NOT_ALLOWED = 405,
  /**
   * status when request has deprecated data, because of version of at least one object is not the last version in database
   */
  CONFLICT = 409,
  /**
   * default server status when server raised not managed error
   */
  INTERNAL_SERVER_ERROR = 500,
  /**
   * status when database server is not reachable
   */
  PROXY_ERROR = 502,
  /**
   * status when server is not currently available
   * example : endpoint disabled during restart of database or update
   */
  SERVICE_UNAVAILABLE = 503
};

#endif
