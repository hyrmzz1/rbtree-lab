#include "rbtree.h" // 쌍따옴표 => 헤더파일이 이 파일과 같은 디렉터리에 있을 때 사용

#include <stdlib.h>

// 여러 개의 tree 생성, 각각 다른 내용들을 저장할 수 있는 RB tree 구조체 생성
rbtree *new_rbtree(void) {
  // tree 구조체 동적 할당
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));  // 하나의 rbtree 구조체 동적 할당 (p는 rbtree 자체를 가리키는 포인터)

  node_t *nil = (node_t *)calloc(1, sizeof(node_t));  // nil은 새로운 nil 노드를 생성하고 그 결과를 가리키는 포인터.
  nil->color = RBTREE_BLACK;  // nil 노드 색상 설정 -> 항상 black
  t->nil = t->root = nil; // nil노드를 rbtree의 'nil', 'root' 필드에 할당. (tree 빈 경우 root는 nil 노드여야 함.)
  return t; // 새로 생성된 rbtree의 포인터가 반환됨. 이후에 p를 사용해 트리 작업 수행 가능
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// 노드 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));  // 새 노드 동적으로 생성
  new_node->key = key;// 노드의 key 입력받기
  new_node->color = RBTREE_RED;  // 삽입하는 노드는 항상 red (삽입 노드가 루트 노드일 때만 black으로 바꿔주기)
  new_node->left = new_node->right = t->nil;  // 삽입한 노드의 자식들 nil(sentinel node)로 설정
  
  // 삽입할 위치 탐색
  node_t *current = t->root;  // 변수 current는 현재 위치를 가리킴. 일단 루트 노드로 초기화

  if(current == t->nil){  // 루트 노드가 비어있으면
    t->root = new_node; // 새 노드를 루트 노드로 지정
    new_node->color = RBTREE_BLACK; // 루트 노드는 검정색
    return new_node;
  }

  while(current != t->nil){ // current가 리프 노드(=nil)에 도달하기 전까지 반복
    // new_node의 key와 current의 key 비교
    if(key < current->key){ // current는 node_t 구조체의 포인터이므로 key 멤버에 접근 가능. new_node의 key가 current의 key보다 작다면 왼쪽 서브트리로
      if(current->left == t->nil){
        current->left = new_node; // 새 노드를 왼쪽 자식으로 추가
        break;
      }
      current = current->left;  // current는 점점 왼쪽 아래로 내려옴
    }
    else if(key > current->key){ // new_node의 key가 current의 key보다 크다면 오른쪽 서브트리로
      if(current->right == t->nil){
        current->right = new_node; // 새 노드를 왼쪽 자식으로 추가
        break;
      }
      current = current->right;
    }
  }
  new_node->parent = current; // 새 노드의 부모 지정

  // 리밸런싱 함수 호출
  rbtree_insert_fixup(t, new_node);

  return t->root; // 삽입 이후 루트 노드 변경될 수 있으므로 트리의 루트 노드 반환
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left; // y의 왼쪽 서브트리를 x의 오른쪽 서브 트리로 옮기기

  if(y->left != t->nil){// y의 왼쪽 서브트리의 부모를 x로 연결
    y->left->parent = x;
  }
  y->parent = x->parent;  // x의 부모를 y의 부모로 연결 (양방향 연결)

  if(x->parent == t->nil){// x의 부모가 없다면 (= x가 루트 노드라면)
    t->root = y;  // y를 트리의 루트로.
  }
  else{
    if(x == x->parent->left){// x가 부모의 왼쪽 노드라면
      x->parent->left = y;  // y는 x의 부모의 왼쪽 노드가 됨
    }
    else{ // x가 부모의 오른쪽 노드라면
      x->parent->right = y; // y는 x의 부모의 오른쪽 노드가 됨
    }
  }
  y->left = x;
  x->parent = y;  // y가 x의 부모가 됨.
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right; // y의 오른쪽 서브트리를 x의 왼쪽 서브트리로 옮기기

  if(y->right != t->nil){// y의 오른쪽 서브트리의 부모를 x로 연결
    y->right->parent = x;
  }
  y->parent = x->parent;  // x의 부모를 y의 부모로 연결 (양방향 연결)

  if(x->parent == t->nil){// x의 부모가 없다면 (= x가 루트 노드라면)
    t->root = y;  // y를 트리의 루트로.
  }
  else{
    if(x == x->parent->left){// x가 부모의 왼쪽 노드라면
      x->parent->left = y;  // y는 x의 부모의 왼쪽 노드가 됨
    }
    else{ // x가 부모의 오른쪽 노드라면
      x->parent->right = y; // y는 x의 부모의 오른쪽 노드가 됨
    }
  }
  y->right = x;
  x->parent = y;  // y가 x의 부모가 됨.

}

// 리밸런싱
void rbtree_insert_fixup(rbtree *t, node_t *node){
}

// 트리 내에 특정 key 값을 갖는 노드가 있는지 탐색 (있으면 node pointer 반환, 없으면 null 반환)
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *current = t->root;  // 루트부터 탐색 시작

  while(current != t->nil){
    if(key == current->key)
      return current;
    else
      current = (key < current->key) ? current->left : current->right;
  }

  if(current == t->nil){
    return NULL;
  }
}

// 최소값 node pointer 반환
node_t *rbtree_min(const rbtree *t) {
  node_t *current = t->root;  // 루트부터 탐색 시작

  if(current == t->nil) // 빈 노드라면
    return current;

  while(current->left != t->nil){
    current = current->left;  // 트리의 맨 왼쪽 노드가 최소
  }
  return current;
}

// 최대값 node pointer 반환
node_t *rbtree_max(const rbtree *t) {
  node_t *current = t->root;

  if(current == t->nil) // 빈 노드라면
    return current;

  while(current->right != t->nil){
    current = current->right; // 트리의 맨 오른쪽 노드가 최대
  }
  return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

// 오름차순으로 키 값 출력하기 위한 중위순회 함수 (트리 구조 확인용)
void inOrder(rbtree *t, node_t *n){
  if(n == t->nil)
      return;
  inOrder(t, n->left);
  printf("Key : %d, Color : %d\n", n->key, n->color);  //color는 열거형 상수이므로 정수형으로 출력 (0:red, 1:black)
  inOrder(t, n->right);
}

/* 참고
pointer -> member
포인터를 통해 구조체나 클래스 내의 멤버(변수 or 함수)에 접근 가능
*/