// generated by F:/vcpkg/installed/x64-windows/scripts/sqlpp11-ddl2cpp F:/Doodle/src/doodle_core/core/sql_file.sql
// F:/Doodle/src/doodle_core/generate/core/sql_sql doodle_database
#ifndef DOODLE_DATABASE_SQL_SQL_H
#define DOODLE_DATABASE_SQL_SQL_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace doodle_database
{
  namespace Context_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct ComHash
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "com_hash";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T comHash;
            T& operator()() { return comHash; }
            const T& operator()() const { return comHash; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct JsonData
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "json_data";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T jsonData;
            T& operator()() { return jsonData; }
            const T& operator()() const { return jsonData; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
  } // namespace Context_

  struct Context: sqlpp::table_t<Context,
               Context_::Id,
               Context_::ComHash,
               Context_::JsonData>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "context";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T context;
        T& operator()() { return context; }
        const T& operator()() const { return context; }
      };
    };
  };
  namespace Entity_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct UuidData
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "uuid_data";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T uuidData;
            T& operator()() { return uuidData; }
            const T& operator()() const { return uuidData; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
    struct UpdateTime
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "update_time";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T updateTime;
            T& operator()() { return updateTime; }
            const T& operator()() const { return updateTime; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
  } // namespace Entity_

  struct Entity: sqlpp::table_t<Entity,
               Entity_::Id,
               Entity_::UuidData,
               Entity_::UpdateTime>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "entity";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T entity;
        T& operator()() { return entity; }
        const T& operator()() const { return entity; }
      };
    };
  };
  namespace ComEntity_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct EntityId
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "entity_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T entityId;
            T& operator()() { return entityId; }
            const T& operator()() const { return entityId; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct ComHash
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "com_hash";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T comHash;
            T& operator()() { return comHash; }
            const T& operator()() const { return comHash; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct JsonData
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "json_data";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T jsonData;
            T& operator()() { return jsonData; }
            const T& operator()() const { return jsonData; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
  } // namespace ComEntity_

  struct ComEntity: sqlpp::table_t<ComEntity,
               ComEntity_::Id,
               ComEntity_::EntityId,
               ComEntity_::ComHash,
               ComEntity_::JsonData>
  {
      struct _alias_t {
          static constexpr const char _literal[] = "com_entity";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T comEntity;
            T& operator()() { return comEntity; }
            const T& operator()() const { return comEntity; }
          };
      };
  };
  namespace DoodleInfo_ {
  struct VersionMajor {
      struct _alias_t {
          static constexpr const char _literal[] = "version_major";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T versionMajor;
            T& operator()() { return versionMajor; }
            const T& operator()() const { return versionMajor; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
  };
  struct VersionMinor {
      struct _alias_t {
          static constexpr const char _literal[] = "version_minor";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T versionMinor;
            T& operator()() { return versionMinor; }
            const T& operator()() const { return versionMinor; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
  };
  }  // namespace DoodleInfo_

  struct DoodleInfo : sqlpp::table_t<DoodleInfo, DoodleInfo_::VersionMajor, DoodleInfo_::VersionMinor> {
      struct _alias_t {
          static constexpr const char _literal[] = "doodle_info";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T doodleInfo;
            T& operator()() { return doodleInfo; }
            const T& operator()() const { return doodleInfo; }
          };
      };
  };
  namespace Usertab_ {
  struct Id {
      struct _alias_t {
          static constexpr const char _literal[] = "id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct UserName {
      struct _alias_t {
        static constexpr const char _literal[] =  "user_name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T userName;
            T& operator()() { return userName; }
            const T& operator()() const { return userName; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct PermissionGroup
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "permission_group";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T permissionGroup;
            T& operator()() { return permissionGroup; }
            const T& operator()() const { return permissionGroup; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
  } // namespace Usertab_

  struct Usertab: sqlpp::table_t<Usertab,
               Usertab_::Id,
               Usertab_::UserName,
               Usertab_::PermissionGroup>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "usertab";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T usertab;
            T& operator()() { return usertab; }
            const T& operator()() const { return usertab; }
          };
    };
  };
  namespace WorkTaskInfo_ {
  struct Id {
    struct _alias_t {
          static constexpr const char _literal[] = "id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct EntityId {
    struct _alias_t {
          static constexpr const char _literal[] = "entity_id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T entityId;
            T& operator()() { return entityId; }
            const T& operator()() const { return entityId; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct ParentId {
    struct _alias_t {
          static constexpr const char _literal[] = "parent_id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T parentId;
            T& operator()() { return parentId; }
            const T& operator()() const { return parentId; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct UserId {
    struct _alias_t {
          static constexpr const char _literal[] = "user_id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T userId;
            T& operator()() { return userId; }
            const T& operator()() const { return userId; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  struct TaskName {
    struct _alias_t {
          static constexpr const char _literal[] = "task_name";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T taskName;
            T& operator()() { return taskName; }
            const T& operator()() const { return taskName; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  struct Region {
    struct _alias_t {
          static constexpr const char _literal[] = "region";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T region;
            T& operator()() { return region; }
            const T& operator()() const { return region; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  struct Abstract {
    struct _alias_t {
          static constexpr const char _literal[] = "abstract";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T abstract;
            T& operator()() { return abstract; }
            const T& operator()() const { return abstract; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  }  // namespace WorkTaskInfo_

  struct WorkTaskInfo
      : sqlpp::table_t<
            WorkTaskInfo, WorkTaskInfo_::Id, WorkTaskInfo_::EntityId, WorkTaskInfo_::ParentId, WorkTaskInfo_::UserId,
            WorkTaskInfo_::TaskName, WorkTaskInfo_::Region, WorkTaskInfo_::Abstract> {
    struct _alias_t {
          static constexpr const char _literal[] = "work_task_info";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T workTaskInfo;
            T& operator()() { return workTaskInfo; }
            const T& operator()() const { return workTaskInfo; }
          };
    };
  };
  namespace TimeWarp_ {
  struct Id {
    struct _alias_t {
          static constexpr const char _literal[] = "id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct EntityId {
    struct _alias_t {
          static constexpr const char _literal[] = "entity_id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T entityId;
            T& operator()() { return entityId; }
            const T& operator()() const { return entityId; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct ParentId {
    struct _alias_t {
          static constexpr const char _literal[] = "parent_id";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T parentId;
            T& operator()() { return parentId; }
            const T& operator()() const { return parentId; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct TimeValue {
    struct _alias_t {
          static constexpr const char _literal[] = "time_value";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T timeValue;
            T& operator()() { return timeValue; }
            const T& operator()() const { return timeValue; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
  };
  }  // namespace TimeWarp_

  struct TimeWarp
      : sqlpp::table_t<TimeWarp, TimeWarp_::Id, TimeWarp_::EntityId, TimeWarp_::ParentId, TimeWarp_::TimeValue> {
    struct _alias_t {
          static constexpr const char _literal[] = "time_warp";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T timeWarp;
            T& operator()() { return timeWarp; }
            const T& operator()() const { return timeWarp; }
          };
    };
  };
  namespace SqliteMaster_ {
  struct Type {
    struct _alias_t {
          static constexpr const char _literal[] = "type";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T type;
            T& operator()() { return type; }
            const T& operator()() const { return type; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  struct Name {
    struct _alias_t {
          static constexpr const char _literal[] = "name";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  struct TblName {
    struct _alias_t {
          static constexpr const char _literal[] = "tbl_name";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T tblName;
            T& operator()() { return tblName; }
            const T& operator()() const { return tblName; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  struct Rootpage {
    struct _alias_t {
          static constexpr const char _literal[] = "rootpage";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T rootpage;
            T& operator()() { return rootpage; }
            const T& operator()() const { return rootpage; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
  };
  struct Sql {
    struct _alias_t {
          static constexpr const char _literal[] = "sql";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T sql;
            T& operator()() { return sql; }
            const T& operator()() const { return sql; }
          };
    };
    using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
  };
  }  // namespace SqliteMaster_

  struct SqliteMaster : sqlpp::table_t<
                            SqliteMaster, SqliteMaster_::Type, SqliteMaster_::Name, SqliteMaster_::TblName,
                            SqliteMaster_::Rootpage, SqliteMaster_::Sql> {
    struct _alias_t {
          static constexpr const char _literal[] = "sqlite_master";
          using _name_t                          = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
          template <typename T>
          struct _member_t {
            T sqliteMaster;
            T& operator()() { return sqliteMaster; }
            const T& operator()() const { return sqliteMaster; }
          };
    };
  };
  }  // namespace doodle_database
#endif
