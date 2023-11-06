#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

/* 
typedef는 자료형을 정의하는 것. (typedef 기존type 별칭;)
typedef int key_t; 라고 정의하면 int a; 대신 key_t a; 라고 선언할 수 있다.
typedef를 이용하면 struct/union(공용체)/enum(열거형) 더 간단히 사용 가능!
*/

// 익명 열거형 정의하는 동시에 변수(color_t) 선언
typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;  // tree 색상

typedef int key_t;  // 각 노드에 저장되는 값

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
  // sentinel : 데이터의 끝을 알리는 데 사용되는 값으로, 정상적인 데이터 값에서는 절대 등장할수 없는 값으로 선택하는 것이 좋다.)
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
