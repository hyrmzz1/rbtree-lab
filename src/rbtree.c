#include "rbtree.h" // 쌍따옴표 => 헤더파일이 이 파일과 같은 디렉터리에 있을 때 사용

#include <stdlib.h>

rbtree *new_rbtree(void) {
  // tree 구조체 동적 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));  // 하나의 rbtree 구조체 동적 할당 (p는 rbtree 자체를 가리키는 포인터)

  // TODO: 여러 개의 tree 생성, 각각 다른 내용들을 저장할 수 있는 RB tree 구조체 생성
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));  // nil은 새로운 nil 노드를 생성하고 그 결과를 가리키는 포인터.
  nil->color = RBTREE_BLACK;  // nil 노드 색상 설정 -> 항상 black
  p->nil = p->root = nil; // nil노드를 rbtree의 'nil', 'root' 필드에 할당. (tree 빈 경우 root는 nil 노드여야 함.)
  return p; // 새로 생성된 rbtree의 포인터가 반환됨. 이후에 p를 사용해 트리 작업 수행 가능
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

/* 참고
pointer -> member
포인터를 통해 구조체나 클래스 내의 멤버(변수 or 함수)에 접근 가능
*/