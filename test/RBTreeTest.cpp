#include "test.h"
#include "base/RBTree.h"
using namespace snow;

#include <math.h>

TEST_SUITE(RBTree);

TEST_CASE(insert_and_find) {
	RBTree<int, int> tree;

	const int n = 100;

	for (int i = 0; i < n; ++i) {
		tree.insert(i, i+123 % 50);
	}

	TEST_EQ(tree.size(), (size_t)n);

	TEST(tree.verify());

	for (int i = 0; i < n; ++i) {
		auto iter = tree.find(i);
		ASSERT(iter != tree.end());
		TEST_NEQ(iter, tree.end());
		TEST_EQ(iter->value, i+123 % 50);
	}

	size_t size = 0;
	for each (iter2, tree) {
		size++;
	}
	TEST_EQ(tree.size(), size);
}

TEST_CASE(erase) {
	RBTree<int, int> tree;

	const int n = 100;

	for (int i = 0; i < n; ++i) {
		tree.insert(i, i+123 % 50);
	}

	TEST_EQ(tree.size(), (size_t)n);

	TEST(tree.verify());

	for (int i = 0; i < n; ++i) {
		if (i % 2)
			tree.erase(i);
	}

	TEST_EQ(tree.size(), (size_t)n/2);
}

TEST_CASE(log_depth) {
	RBTree<int, int> tree;

	const int n = 1000;
	
	for (int i = 0; i < n; ++i) {
		tree.insert(i, i+123 % 500);
	}

	double height = (double)tree.height();

	double log2_n = 2.0 * log((double)tree.size()) / log(2.0);

	TEST(height <= ceil(log2_n));
}

TEST_CASE(index_operator) {
	RBTree<int, int> tree;

	const int n = 100;

	for (int i = n; i > 0; --i) {
		tree[i] = i + 123 % 50;
	}

	TEST_EQ(tree.size(), n);

	TEST_EQ(tree[123], int());

	TEST(tree.verify());

	for (int i = n; i > 0; --i) {
		TEST_EQ(tree[i], i + 123 % 50);
	}
}

