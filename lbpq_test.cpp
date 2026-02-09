#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  // 连接信息字符串
  const char *conninfo = "dbname=la_liga user=xwd password=tmdgnnwscjld "
                         "hostaddr=127.0.0.1 port=5432";

  // 建立连接
  PGconn *conn = PQconnectdb(conninfo);

  // 检查连接状态
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "连接失败: %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
  }

  // 执行查询
  PGresult *res = PQexec(conn, "SELECT * FROM league_standings");

  // 检查查询结果状态
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "查询失败: %s\n", PQerrorMessage(conn));
    PQclear(res);
    PQfinish(conn);
    exit(1);
  }

  // 处理查询结果
  int rows = PQntuples(res);
  int cols = PQnfields(res);

  printf("查询成功，共找到 %d 行 %d 列。\n", rows, cols);

  // 打印每一行的数据
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%s\t", PQgetvalue(res, i, j));
    }
    printf("\n");
  }

  // 清理结果集
  PQclear(res);

  // 关闭连接
  PQfinish(conn);

  return 0;
}
