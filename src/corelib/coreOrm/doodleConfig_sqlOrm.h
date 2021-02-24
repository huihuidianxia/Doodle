// generated by sqlpp11-ddl2cpp 001-doodleConfig.sql doodleConfig_sqlOrm doodle
#ifndef DOODLE_DOODLECONFIG_SQLORM_H
#define DOODLE_DOODLECONFIG_SQLORM_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace doodle
{
  namespace DoodleConfig_
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
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct DoodleKey
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "doodleKey";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T doodleKey;
            T& operator()() { return doodleKey; }
            const T& operator()() const { return doodleKey; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct DoodleValue
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "doodleValue";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T doodleValue;
            T& operator()() { return doodleValue; }
            const T& operator()() const { return doodleValue; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
  } // namespace DoodleConfig_

  struct DoodleConfig: sqlpp::table_t<DoodleConfig,
               DoodleConfig_::Id,
               DoodleConfig_::DoodleKey,
               DoodleConfig_::DoodleValue>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "doodleConfig";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T doodleConfig;
        T& operator()() { return doodleConfig; }
        const T& operator()() const { return doodleConfig; }
      };
    };
  };
} // namespace doodle
#endif
