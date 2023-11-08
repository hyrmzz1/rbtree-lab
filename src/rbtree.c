#include "rbtree.h" // 쌍따옴표 => 헤더파일이 이 파일과 같은 디렉터리에 있을 때 사용

#include <stdlib.h>

// 여러 개의 tree 생성, 각각 다른 내용들을 저장할 수 있는 RB tree 구조체 생성
rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));  // 하나의 rbtree 구조체 동적 할당 (p는 rbtree 자체를 가리키는 포인터)

  node_t *nil = (node_t *)calloc(1, sizeof(node_t));  // nil은 새로운 nil 노드를 생성하고 그 결과를 가리키는 포인터.
  nil->color = RBTREE_BLACK;  // nil 노드 색상 설정 -> 항상 black
  t->nil = t->root = nil; // nil노드를 rbtree의 'nil', 'root' 필드에 할당. (tree 빈 경우 root는 nil 노드여야 함.)
  return t; // 새로 생성된 rbtree의 포인터가 반환됨. 이후에 p를 사용해 트리 작업 수행 가능
}

// RB tree 구조체의 노드들이 차지했던 메모리 반환 (valgrind로 나타나면 안됨)
void delete_all_node(rbtree *t, node_t *n) {
  if(n == t->nil)
    return;
  delete_all_node(t, n->left);
  delete_all_node(t, n->right);
  free(n);
}

// RB tree 구조체가 차지했던 메모리 반환 (valgrind로 나타나면 안됨)
void delete_rbtree(rbtree *t) {
  delete_all_node(t, t->root);
  if(t->root == t->nil){  // 트리 비어있으면 (노드들 다 반환되었으면)
    free(t->nil);
    free(t);
  }
}

// 노드 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));  // 새 노드 동적으로 생성
  new_node->key = key;// 노드의 key 입력받기
  new_node->color = RBTREE_RED;  // 삽입하는 노드는 항상 red (삽입 노드가 루트 노드일 때만 black으로 바꿔주기)
  new_node->left = new_node->right = t->nil;  // 삽입한 노드의 자식들 nil(sentinel node)로 설정
  
  // 삽입할 위치 탐색
  node_t *temp = t->root;  // 변수 temp는 현재 위치를 가리킴. 일단 루트 노드로 초기화

  if(temp == t->nil){  // 루트 노드가 비어있으면
    t->root = new_node; // 새 노드를 루트 노드로 지정
    new_node->color = RBTREE_BLACK; // 루트 노드는 검정색
    return new_node;
  }

  while(temp != t->nil){ // temp가 리프 노드(=nil)에 도달하기 전까지 반복
    // new_node의 key와 temp의 key 비교
    if(key < temp->key){ // temp는 node_t 구조체의 포인터이므로 key 멤버에 접근 가능. new_node의 key가 temp의 key보다 작다면 왼쪽 서브트리로
      if(temp->left == t->nil){
        temp->left = new_node; // 새 노드를 왼쪽 자식으로 추가
        break;
      }
      temp = temp->left;  // temp는 점점 왼쪽 아래로 내려옴
    }
    else if(key > temp->key){ // new_node의 key가 temp의 key보다 크다면 오른쪽 서브트리로
      if(temp->right == t->nil){
        temp->right = new_node; // 새 노드를 왼쪽 자식으로 추가
        break;
      }
      temp = temp->right;
    }
  }
  new_node->parent = temp; // 새 노드의 부모 지정

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
void rbtree_insert_fixup(rbtree *t, node_t *n){ 
  node_t *parent = n->parent;
  node_t *grand_parent = parent->parent;
  node_t *uncle = (parent == grand_parent->left) ? grand_parent->right : grand_parent->left;

  // 회전으로 인해 red 노드가 루트 노드가 된다면 black으로 색상 변경
  if(n == t->root){
    n->color = RBTREE_BLACK;
    return;
  }

  // 리밸런싱 이유: red가 연속 (속성 4 위반) => 새 노드는 red니까 부모 노드 black이면 연속 X
  if(parent->color == RBTREE_BLACK)
    return;

  while(parent->color == RBTREE_RED){
    if(parent == grand_parent->left){ // uncle == grandparent->right
      // case 1
      if(uncle->color == RBTREE_RED){
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        n = grand_parent;
      }
      // case 2 ('<' 모양)
      else{
        if(n == parent->right){
          n = parent;
          left_rotate(t, n);
        }
        // case 3  ('/' 모양)
        parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
    }
    else{ // uncle == grandparent->left
      if(uncle->color == RBTREE_RED){
          parent->color = RBTREE_BLACK;
          uncle->color = RBTREE_BLACK;
          grand_parent->color = RBTREE_RED;
          n = grand_parent;
        }
        // case 2 ('<' 모양)
        else{
          if(n == parent->left){
            n = parent;
            right_rotate(t, n);
          }
          // case 3  ('/' 모양)
          parent->color = RBTREE_BLACK;
          grand_parent->color = RBTREE_RED;
          left_rotate(t, grand_parent);
        }
    }
  }
  t->root->color = RBTREE_BLACK;
}

// 트리 내에 특정 key 값을 갖는 노드가 있는지 탐색 (있으면 node pointer 반환, 없으면 null 반환)
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *temp = t->root;  // 루트부터 탐색 시작

  while(temp != t->nil){
    if(key == temp->key)
      return temp;
    else
      temp = (key < temp->key) ? temp->left : temp->right;
  }

  if(temp == t->nil){
    return NULL;
  }
}

// 최소값 node pointer 반환
node_t *rbtree_min(const rbtree *t) {
  node_t *temp = t->root;  // 루트부터 탐색 시작

  if(temp == t->nil) // 빈 노드라면
    return temp;

  while(temp->left != t->nil){
    temp = temp->left;  // 트리의 맨 왼쪽 노드가 최소
  }
  return temp;
}

// 최대값 node pointer 반환
node_t *rbtree_max(const rbtree *t) {
  node_t *temp = t->root;

  if(temp == t->nil) // 빈 노드라면
    return temp;

  while(temp->right != t->nil){
    temp = temp->right; // 트리의 맨 오른쪽 노드가 최대
  }
  return temp;
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