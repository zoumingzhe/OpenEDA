/* @file  hv_tree.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "edi/util/hv_tree.h"

// for testing
int main(int argc, char **argv) {
// int test_hv_tree() {
  // test rect
  ::EDI_NAMESPACE::HVRect a(1, 2, 3, 4);
  ::EDI_NAMESPACE::HVRect b(0, 0, 0, 0);
  printf("a = %d, %d, %d, %d\n", a.getLLX(), a.getLLY(), a.getURX(), a.getURY());
  printf("b = %d, %d, %d, %d\n", b.getLLX(), b.getLLY(), b.getURX(), b.getURY());
  //b.setRect(a);
  //b = a;
  a = b;
  printf("a = %d, %d, %d, %d\n", a.getLLX(), a.getLLY(), a.getURX(), a.getURY());
  printf("b = %d, %d, %d, %d\n", b.getLLX(), b.getLLY(), b.getURX(), b.getURY());
  if (a == b) {
    printf("you are right, a is equal b\n");
  }
  // test ::EDI_NAMESPACE::HVCutNode
  ::EDI_NAMESPACE::HVCutNode<::EDI_NAMESPACE::HVRect> cut_root;
  cut_root.setDir(::EDI_NAMESPACE::Horizontal); // CutNode's initial dir is from TreeNode
  ::EDI_NAMESPACE::HVRect a1(5, 8, 16, 15);
  ::EDI_NAMESPACE::HVRect a2(7, 20, 10, 23);
  ::EDI_NAMESPACE::HVRect a3(6, 28, 11, 35);
  ::EDI_NAMESPACE::HVRect a4(8, 50, 19, 56);
  ::EDI_NAMESPACE::HVRect a5(5, 53, 17, 58);
  ::EDI_NAMESPACE::HVRect a6(6, 70, 9, 78);
  ::EDI_NAMESPACE::HVRect a7(3, 5, 20, 7);
  ::EDI_NAMESPACE::HVRect a8(1, 5, 22, 9);
  ::EDI_NAMESPACE::HVRect a9(8, 80, 12, 85);
  cut_root.insert(&a1);
  cut_root.insert(&a2);
  cut_root.insert(&a3);
  cut_root.insert(&a4);
  cut_root.insert(&a5);
  cut_root.insert(&a6);
  cut_root.insert(&a7);
  cut_root.insert(&a8);
  cut_root.insert(&a9);
  // traverse();
  ::EDI_NAMESPACE::HVTree<::EDI_NAMESPACE::HVRect> pg_tree;
  // may find way to set cut direction, e.g. by routing direction, layer, pin/instance etc.
  // pg_tree.setThreshold(10);
  // pg_tree.setCutDir(Vertical); // use setDefaultDir() for direction
  ::EDI_NAMESPACE::HVRect b1(1, 8, 3, 15);
  ::EDI_NAMESPACE::HVRect b2(2, 20, 7, 23);
  ::EDI_NAMESPACE::HVRect b3(10, 28, 17, 35);
  ::EDI_NAMESPACE::HVRect b4(12, 50, 15, 56);
  ::EDI_NAMESPACE::HVRect b5(5, 53, 7, 58);
  ::EDI_NAMESPACE::HVRect b6(10, 53, 17, 58);
  pg_tree.insert(&a1);
  pg_tree.insert(&b1);
  pg_tree.insert(&a2);
  pg_tree.insert(&b2);
  pg_tree.insert(&a3);
  pg_tree.insert(&b3);
  pg_tree.insert(&a5);
  pg_tree.insert(&b4);
  pg_tree.insert(&a6);
  pg_tree.insert(&b5);
  pg_tree.insert(&b6);
  //printf("\nTraverse PreOrder\n");
  //pg_tree.traverse(PreOrder);
  //printf("\nTraverse InOrder\n");
  //pg_tree.traverse(InOrder);
  //printf("\nTraverse PostOrder\n");
  //pg_tree.traverse(PostOrder);

  // more insert
  ::EDI_NAMESPACE::HVRect b7(1, 70, 2, 71);
  ::EDI_NAMESPACE::HVRect b8(10, 70, 12, 71);
  ::EDI_NAMESPACE::HVRect b9(1, 38, 2, 40);
  ::EDI_NAMESPACE::HVRect b10(10, 47, 12, 49);
  ::EDI_NAMESPACE::HVRect b11(15, 58, 17, 62);
  pg_tree.insert(&b7);
  pg_tree.insert(&b8);
  pg_tree.insert(&b9);
  pg_tree.insert(&b10);
  pg_tree.insert(&b11);
  printf("\nTraverse PreOrder\n");
  pg_tree.traverse(::EDI_NAMESPACE::PreOrder);

  // remove
  // test remove 1
  //pg_tree.remove(b11);
  //pg_tree.remove(b10);
  // test remove 2
  //pg_tree.remove(b9);
  //pg_tree.remove(b1);
  //pg_tree.remove(b2);
  //pg_tree.remove(b5);
  //pg_tree.remove(b7);
  // test remove 3
  //pg_tree.remove(a1);
  //pg_tree.remove(a2);
  //pg_tree.remove(a3);
  //pg_tree.remove(a5);
  //pg_tree.remove(a6);
  //printf("\nTraverse PreOrder\n");
  //pg_tree.traverse(PreOrder);

  // test search
  std::vector<::EDI_NAMESPACE::HVRect *> my_search_result;
  ::EDI_NAMESPACE::HVRect my_search_rect(10, 40, 20, 60);
  //::EDI_NAMESPACE::HVRect my_search_rect(1, 5, 8, 15);
  //::EDI_NAMESPACE::HVRect my_search_rect(1, 20, 17, 70);
  pg_tree.search(my_search_rect, &my_search_result);
  printf("search result:\n");
  for (unsigned int i = 0; i < my_search_result.size(); i++) {
    ::EDI_NAMESPACE::HVRect rect_i = getObjRect(my_search_result[i]);
    printf("%d, %d, %d, %d\n", rect_i.getLLX(), rect_i.getLLY(), rect_i.getURX(), rect_i.getURY());
  }

  printf("Oh! Yeah!\n");
  return 0;
}
