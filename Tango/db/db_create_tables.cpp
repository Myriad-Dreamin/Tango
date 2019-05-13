
#include <QSqlDatabase>
#include "db_create_tables.h"

bool create_authors_table()
{
    static const char *create_command =
        "CREATE TABLE IF NOT EXISTS `authors` ("
        "  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "  `name` varchar(45) NOT NULL,"
        "   `exp` int(10) unsigned NOT NULL DEFAULT '0',"
        "   `level` int(10) unsigned NOT NULL DEFAULT '0',"
        "   `tango_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "   `misson_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "   `motto` longtext,"
        "   PRIMARY KEY (`id`),"
        "   UNIQUE KEY `name_UNIQUE` (`name`)"
        " ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='authors that produce good tango list for users'";
    QSqlQuery query(QSqlDatabase::database());
    query.exec("set names 'utf8'");

    if (! query.exec(create_command)) {
        qDebug() << "create authors table failed" << query.lastError().text();
        return false;
    }
    return true;
}

