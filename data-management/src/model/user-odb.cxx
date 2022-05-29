// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#include <odb/pre.hxx>

#include "user-odb.hxx"

#include <cassert>
#include <cstring>  // std::memcpy

#include <odb/schema-catalog-impl.hxx>

#include <odb/sqlite/traits.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/sqlite/transaction.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/statement-cache.hxx>
#include <odb/sqlite/simple-object-statements.hxx>
#include <odb/sqlite/view-statements.hxx>
#include <odb/sqlite/container-statements.hxx>
#include <odb/sqlite/exceptions.hxx>
#include <odb/sqlite/simple-object-result.hxx>
#include <odb/sqlite/view-result.hxx>

namespace odb
{
  // user
  //

  struct access::object_traits_impl< ::user, id_sqlite >::extra_statement_cache_type
  {
    extra_statement_cache_type (
      sqlite::connection&,
      image_type&,
      id_image_type&,
      sqlite::binding&,
      sqlite::binding&)
    {
    }
  };

  access::object_traits_impl< ::user, id_sqlite >::id_type
  access::object_traits_impl< ::user, id_sqlite >::
  id (const id_image_type& i)
  {
    sqlite::database* db (0);
    ODB_POTENTIALLY_UNUSED (db);

    id_type id;
    {
      sqlite::value_traits<
          long unsigned int,
          sqlite::id_integer >::set_value (
        id,
        i.id_value,
        i.id_null);
    }

    return id;
  }

  access::object_traits_impl< ::user, id_sqlite >::id_type
  access::object_traits_impl< ::user, id_sqlite >::
  id (const image_type& i)
  {
    sqlite::database* db (0);
    ODB_POTENTIALLY_UNUSED (db);

    id_type id;
    {
      sqlite::value_traits<
          long unsigned int,
          sqlite::id_integer >::set_value (
        id,
        i.id_value,
        i.id_null);
    }

    return id;
  }

  bool access::object_traits_impl< ::user, id_sqlite >::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // id_
    //
    t[0UL] = false;

    // login_
    //
    if (t[1UL])
    {
      i.login_value.capacity (i.login_size);
      grew = true;
    }

    // password_
    //
    if (t[2UL])
    {
      i.password_value.capacity (i.password_size);
      grew = true;
    }

    // saltUser_
    //
    if (t[3UL])
    {
      i.saltUser_value.capacity (i.saltUser_size);
      grew = true;
    }

    // saltSession_
    //
    if (t[4UL])
    {
      i.saltSession_value.capacity (i.saltSession_size);
      grew = true;
    }

    // token_
    //
    if (t[5UL])
    {
      i.token_value.capacity (i.token_size);
      grew = true;
    }

    return grew;
  }

  void access::object_traits_impl< ::user, id_sqlite >::
  bind (sqlite::bind* b,
        image_type& i,
        sqlite::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace sqlite;

    std::size_t n (0);

    // id_
    //
    if (sk != statement_update)
    {
      b[n].type = sqlite::bind::integer;
      b[n].buffer = &i.id_value;
      b[n].is_null = &i.id_null;
      n++;
    }

    // login_
    //
    b[n].type = sqlite::image_traits<
      ::std::string,
      sqlite::id_text>::bind_value;
    b[n].buffer = i.login_value.data ();
    b[n].size = &i.login_size;
    b[n].capacity = i.login_value.capacity ();
    b[n].is_null = &i.login_null;
    n++;

    // password_
    //
    b[n].type = sqlite::image_traits<
      ::std::string,
      sqlite::id_text>::bind_value;
    b[n].buffer = i.password_value.data ();
    b[n].size = &i.password_size;
    b[n].capacity = i.password_value.capacity ();
    b[n].is_null = &i.password_null;
    n++;

    // saltUser_
    //
    b[n].type = sqlite::image_traits<
      ::std::string,
      sqlite::id_text>::bind_value;
    b[n].buffer = i.saltUser_value.data ();
    b[n].size = &i.saltUser_size;
    b[n].capacity = i.saltUser_value.capacity ();
    b[n].is_null = &i.saltUser_null;
    n++;

    // saltSession_
    //
    b[n].type = sqlite::image_traits<
      ::std::string,
      sqlite::id_text>::bind_value;
    b[n].buffer = i.saltSession_value.data ();
    b[n].size = &i.saltSession_size;
    b[n].capacity = i.saltSession_value.capacity ();
    b[n].is_null = &i.saltSession_null;
    n++;

    // token_
    //
    b[n].type = sqlite::image_traits<
      ::std::string,
      sqlite::id_text>::bind_value;
    b[n].buffer = i.token_value.data ();
    b[n].size = &i.token_size;
    b[n].capacity = i.token_value.capacity ();
    b[n].is_null = &i.token_null;
    n++;
  }

  void access::object_traits_impl< ::user, id_sqlite >::
  bind (sqlite::bind* b, id_image_type& i)
  {
    std::size_t n (0);
    b[n].type = sqlite::bind::integer;
    b[n].buffer = &i.id_value;
    b[n].is_null = &i.id_null;
  }

  bool access::object_traits_impl< ::user, id_sqlite >::
  init (image_type& i,
        const object_type& o,
        sqlite::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace sqlite;

    bool grew (false);

    // id_
    //
    if (sk == statement_insert)
    {
      long unsigned int const& v =
        o.id_;

      bool is_null (false);
      sqlite::value_traits<
          long unsigned int,
          sqlite::id_integer >::set_image (
        i.id_value,
        is_null,
        v);
      i.id_null = is_null;
    }

    // login_
    //
    {
      ::std::string const& v =
        o.login_;

      bool is_null (false);
      std::size_t cap (i.login_value.capacity ());
      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_image (
        i.login_value,
        i.login_size,
        is_null,
        v);
      i.login_null = is_null;
      grew = grew || (cap != i.login_value.capacity ());
    }

    // password_
    //
    {
      ::std::string const& v =
        o.password_;

      bool is_null (false);
      std::size_t cap (i.password_value.capacity ());
      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_image (
        i.password_value,
        i.password_size,
        is_null,
        v);
      i.password_null = is_null;
      grew = grew || (cap != i.password_value.capacity ());
    }

    // saltUser_
    //
    {
      ::std::string const& v =
        o.saltUser_;

      bool is_null (false);
      std::size_t cap (i.saltUser_value.capacity ());
      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_image (
        i.saltUser_value,
        i.saltUser_size,
        is_null,
        v);
      i.saltUser_null = is_null;
      grew = grew || (cap != i.saltUser_value.capacity ());
    }

    // saltSession_
    //
    {
      ::std::string const& v =
        o.saltSession_;

      bool is_null (false);
      std::size_t cap (i.saltSession_value.capacity ());
      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_image (
        i.saltSession_value,
        i.saltSession_size,
        is_null,
        v);
      i.saltSession_null = is_null;
      grew = grew || (cap != i.saltSession_value.capacity ());
    }

    // token_
    //
    {
      ::std::string const& v =
        o.token_;

      bool is_null (false);
      std::size_t cap (i.token_value.capacity ());
      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_image (
        i.token_value,
        i.token_size,
        is_null,
        v);
      i.token_null = is_null;
      grew = grew || (cap != i.token_value.capacity ());
    }

    return grew;
  }

  void access::object_traits_impl< ::user, id_sqlite >::
  init (object_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (db);

    // id_
    //
    {
      long unsigned int& v =
        o.id_;

      sqlite::value_traits<
          long unsigned int,
          sqlite::id_integer >::set_value (
        v,
        i.id_value,
        i.id_null);
    }

    // login_
    //
    {
      ::std::string& v =
        o.login_;

      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_value (
        v,
        i.login_value,
        i.login_size,
        i.login_null);
    }

    // password_
    //
    {
      ::std::string& v =
        o.password_;

      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_value (
        v,
        i.password_value,
        i.password_size,
        i.password_null);
    }

    // saltUser_
    //
    {
      ::std::string& v =
        o.saltUser_;

      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_value (
        v,
        i.saltUser_value,
        i.saltUser_size,
        i.saltUser_null);
    }

    // saltSession_
    //
    {
      ::std::string& v =
        o.saltSession_;

      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_value (
        v,
        i.saltSession_value,
        i.saltSession_size,
        i.saltSession_null);
    }

    // token_
    //
    {
      ::std::string& v =
        o.token_;

      sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::set_value (
        v,
        i.token_value,
        i.token_size,
        i.token_null);
    }
  }

  void access::object_traits_impl< ::user, id_sqlite >::
  init (id_image_type& i, const id_type& id)
  {
    {
      bool is_null (false);
      sqlite::value_traits<
          long unsigned int,
          sqlite::id_integer >::set_image (
        i.id_value,
        is_null,
        id);
      i.id_null = is_null;
    }
  }

  const char access::object_traits_impl< ::user, id_sqlite >::persist_statement[] =
  "INSERT INTO \"user\" "
  "(\"id\", "
  "\"login\", "
  "\"password\", "
  "\"saltUser\", "
  "\"saltSession\", "
  "\"token\") "
  "VALUES "
  "(?, ?, ?, ?, ?, ?)";

  const char access::object_traits_impl< ::user, id_sqlite >::find_statement[] =
  "SELECT "
  "\"user\".\"id\", "
  "\"user\".\"login\", "
  "\"user\".\"password\", "
  "\"user\".\"saltUser\", "
  "\"user\".\"saltSession\", "
  "\"user\".\"token\" "
  "FROM \"user\" "
  "WHERE \"user\".\"id\"=?";

  const char access::object_traits_impl< ::user, id_sqlite >::update_statement[] =
  "UPDATE \"user\" "
  "SET "
  "\"login\"=?, "
  "\"password\"=?, "
  "\"saltUser\"=?, "
  "\"saltSession\"=?, "
  "\"token\"=? "
  "WHERE \"id\"=?";

  const char access::object_traits_impl< ::user, id_sqlite >::erase_statement[] =
  "DELETE FROM \"user\" "
  "WHERE \"id\"=?";

  const char access::object_traits_impl< ::user, id_sqlite >::query_statement[] =
  "SELECT "
  "\"user\".\"id\", "
  "\"user\".\"login\", "
  "\"user\".\"password\", "
  "\"user\".\"saltUser\", "
  "\"user\".\"saltSession\", "
  "\"user\".\"token\" "
  "FROM \"user\"";

  const char access::object_traits_impl< ::user, id_sqlite >::erase_query_statement[] =
  "DELETE FROM \"user\"";

  const char access::object_traits_impl< ::user, id_sqlite >::table_name[] =
  "\"user\"";

  void access::object_traits_impl< ::user, id_sqlite >::
  persist (database& db, object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace sqlite;

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::pre_persist);

    image_type& im (sts.image ());
    binding& imb (sts.insert_image_binding ());

    if (init (im, obj, statement_insert))
      im.version++;

    im.id_null = true;

    if (im.version != sts.insert_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_insert);
      sts.insert_image_version (im.version);
      imb.version++;
    }

    {
      id_image_type& i (sts.id_image ());
      binding& b (sts.id_image_binding ());
      if (i.version != sts.id_image_version () || b.version == 0)
      {
        bind (b.bind, i);
        sts.id_image_version (i.version);
        b.version++;
      }
    }

    insert_statement& st (sts.persist_statement ());
    if (!st.execute ())
      throw object_already_persistent ();

    obj.id_ = id (sts.id_image ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::post_persist);
  }

  void access::object_traits_impl< ::user, id_sqlite >::
  update (database& db, const object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace sqlite;
    using sqlite::update_statement;

    callback (db, obj, callback_event::pre_update);

    sqlite::transaction& tr (sqlite::transaction::current ());
    sqlite::connection& conn (tr.connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    const id_type& id (
      obj.id_);
    id_image_type& idi (sts.id_image ());
    init (idi, id);

    image_type& im (sts.image ());
    if (init (im, obj, statement_update))
      im.version++;

    bool u (false);
    binding& imb (sts.update_image_binding ());
    if (im.version != sts.update_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_update);
      sts.update_image_version (im.version);
      imb.version++;
      u = true;
    }

    binding& idb (sts.id_image_binding ());
    if (idi.version != sts.update_id_image_version () ||
        idb.version == 0)
    {
      if (idi.version != sts.id_image_version () ||
          idb.version == 0)
      {
        bind (idb.bind, idi);
        sts.id_image_version (idi.version);
        idb.version++;
      }

      sts.update_id_image_version (idi.version);

      if (!u)
        imb.version++;
    }

    update_statement& st (sts.update_statement ());
    if (st.execute () == 0)
      throw object_not_persistent ();

    callback (db, obj, callback_event::post_update);
    pointer_cache_traits::update (db, obj);
  }

  void access::object_traits_impl< ::user, id_sqlite >::
  erase (database& db, const id_type& id)
  {
    using namespace sqlite;

    ODB_POTENTIALLY_UNUSED (db);

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    id_image_type& i (sts.id_image ());
    init (i, id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    if (sts.erase_statement ().execute () != 1)
      throw object_not_persistent ();

    pointer_cache_traits::erase (db, id);
  }

  access::object_traits_impl< ::user, id_sqlite >::pointer_type
  access::object_traits_impl< ::user, id_sqlite >::
  find (database& db, const id_type& id)
  {
    using namespace sqlite;

    {
      pointer_type p (pointer_cache_traits::find (db, id));

      if (!pointer_traits::null_ptr (p))
        return p;
    }

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    if (l.locked ())
    {
      if (!find_ (sts, &id))
        return pointer_type ();
    }

    pointer_type p (
      access::object_factory<object_type, pointer_type>::create ());
    pointer_traits::guard pg (p);

    pointer_cache_traits::insert_guard ig (
      pointer_cache_traits::insert (db, id, p));

    object_type& obj (pointer_traits::get_ref (p));

    if (l.locked ())
    {
      select_statement& st (sts.find_statement ());
      ODB_POTENTIALLY_UNUSED (st);

      callback (db, obj, callback_event::pre_load);
      init (obj, sts.image (), &db);
      load_ (sts, obj, false);
      sts.load_delayed (0);
      l.unlock ();
      callback (db, obj, callback_event::post_load);
      pointer_cache_traits::load (ig.position ());
    }
    else
      sts.delay_load (id, obj, ig.position ());

    ig.release ();
    pg.release ();
    return p;
  }

  bool access::object_traits_impl< ::user, id_sqlite >::
  find (database& db, const id_type& id, object_type& obj)
  {
    using namespace sqlite;

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    if (!find_ (sts, &id))
      return false;

    select_statement& st (sts.find_statement ());
    ODB_POTENTIALLY_UNUSED (st);

    reference_cache_traits::position_type pos (
      reference_cache_traits::insert (db, id, obj));
    reference_cache_traits::insert_guard ig (pos);

    callback (db, obj, callback_event::pre_load);
    init (obj, sts.image (), &db);
    load_ (sts, obj, false);
    sts.load_delayed (0);
    l.unlock ();
    callback (db, obj, callback_event::post_load);
    reference_cache_traits::load (pos);
    ig.release ();
    return true;
  }

  bool access::object_traits_impl< ::user, id_sqlite >::
  reload (database& db, object_type& obj)
  {
    using namespace sqlite;

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    const id_type& id  (
      obj.id_);

    if (!find_ (sts, &id))
      return false;

    select_statement& st (sts.find_statement ());
    ODB_POTENTIALLY_UNUSED (st);

    callback (db, obj, callback_event::pre_load);
    init (obj, sts.image (), &db);
    load_ (sts, obj, true);
    sts.load_delayed (0);
    l.unlock ();
    callback (db, obj, callback_event::post_load);
    return true;
  }

  bool access::object_traits_impl< ::user, id_sqlite >::
  find_ (statements_type& sts,
         const id_type* id)
  {
    using namespace sqlite;

    id_image_type& i (sts.id_image ());
    init (i, *id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    select_statement& st (sts.find_statement ());

    st.execute ();
    auto_result ar (st);
    select_statement::result r (st.fetch ());

    if (r == select_statement::truncated)
    {
      if (grow (im, sts.select_image_truncated ()))
        im.version++;

      if (im.version != sts.select_image_version ())
      {
        bind (imb.bind, im, statement_select);
        sts.select_image_version (im.version);
        imb.version++;
        st.refetch ();
      }
    }

    return r != select_statement::no_data;
  }

  result< access::object_traits_impl< ::user, id_sqlite >::object_type >
  access::object_traits_impl< ::user, id_sqlite >::
  query (database&, const query_base_type& q)
  {
    using namespace sqlite;
    using odb::details::shared;
    using odb::details::shared_ptr;

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());

    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    std::string text (query_statement);
    if (!q.empty ())
    {
      text += " ";
      text += q.clause ();
    }

    q.init_parameters ();
    shared_ptr<select_statement> st (
      new (shared) select_statement (
        conn,
        text,
        false,
        true,
        q.parameters_binding (),
        imb));

    st->execute ();

    shared_ptr< odb::object_result_impl<object_type> > r (
      new (shared) sqlite::object_result_impl<object_type> (
        q, st, sts, 0));

    return result<object_type> (r);
  }

  unsigned long long access::object_traits_impl< ::user, id_sqlite >::
  erase_query (database&, const query_base_type& q)
  {
    using namespace sqlite;

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());

    std::string text (erase_query_statement);
    if (!q.empty ())
    {
      text += ' ';
      text += q.clause ();
    }

    q.init_parameters ();
    delete_statement st (
      conn,
      text,
      q.parameters_binding ());

    return st.execute ();
  }

  // user_stat
  //

  bool access::view_traits_impl< ::user_stat, id_sqlite >::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // count
    //
    t[0UL] = false;

    return grew;
  }

  void access::view_traits_impl< ::user_stat, id_sqlite >::
  bind (sqlite::bind* b,
        image_type& i)
  {
    using namespace sqlite;

    sqlite::statement_kind sk (statement_select);
    ODB_POTENTIALLY_UNUSED (sk);

    std::size_t n (0);

    // count
    //
    b[n].type = sqlite::bind::integer;
    b[n].buffer = &i.count_value;
    b[n].is_null = &i.count_null;
    n++;
  }

  void access::view_traits_impl< ::user_stat, id_sqlite >::
  init (view_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (db);

    // count
    //
    {
      ::std::size_t& v =
        o.count;

      sqlite::value_traits<
          ::std::size_t,
          sqlite::id_integer >::set_value (
        v,
        i.count_value,
        i.count_null);
    }
  }

  access::view_traits_impl< ::user_stat, id_sqlite >::query_base_type
  access::view_traits_impl< ::user_stat, id_sqlite >::
  query_statement (const query_base_type& q)
  {
    query_base_type r (
      "SELECT "
      "count(\"user\".\"id\") ");

    r += "FROM \"user\"";

    if (!q.empty ())
    {
      r += " ";
      r += q.clause_prefix ();
      r += q;
    }

    return r;
  }

  result< access::view_traits_impl< ::user_stat, id_sqlite >::view_type >
  access::view_traits_impl< ::user_stat, id_sqlite >::
  query (database&, const query_base_type& q)
  {
    using namespace sqlite;
    using odb::details::shared;
    using odb::details::shared_ptr;

    sqlite::connection& conn (
      sqlite::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_view<view_type> ());

    image_type& im (sts.image ());
    binding& imb (sts.image_binding ());

    if (im.version != sts.image_version () || imb.version == 0)
    {
      bind (imb.bind, im);
      sts.image_version (im.version);
      imb.version++;
    }

    const query_base_type& qs (query_statement (q));
    qs.init_parameters ();
    shared_ptr<select_statement> st (
      new (shared) select_statement (
        conn,
        qs.clause (),
        false,
        true,
        qs.parameters_binding (),
        imb));

    st->execute ();

    shared_ptr< odb::view_result_impl<view_type> > r (
      new (shared) sqlite::view_result_impl<view_type> (
        qs, st, sts, 0));

    return result<view_type> (r);
  }
}

namespace odb
{
  static bool
  create_schema (database& db, unsigned short pass, bool drop)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (pass);
    ODB_POTENTIALLY_UNUSED (drop);

    if (drop)
    {
      switch (pass)
      {
        case 1:
        {
          return true;
        }
        case 2:
        {
          db.execute ("DROP TABLE IF EXISTS \"user\"");
          return false;
        }
      }
    }
    else
    {
      switch (pass)
      {
        case 1:
        {
          db.execute ("CREATE TABLE \"user\" (\n"
                      "  \"id\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\n"
                      "  \"login\" TEXT NOT NULL,\n"
                      "  \"password\" TEXT NOT NULL,\n"
                      "  \"saltUser\" TEXT NOT NULL,\n"
                      "  \"saltSession\" TEXT NOT NULL,\n"
                      "  \"token\" TEXT NOT NULL)");
          return false;
        }
      }
    }

    return false;
  }

  static const schema_catalog_create_entry
  create_schema_entry_ (
    id_sqlite,
    "",
    &create_schema);
}

#include <odb/post.hxx>
