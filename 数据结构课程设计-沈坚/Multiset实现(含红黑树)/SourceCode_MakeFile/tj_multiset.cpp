//1853790 数强 庄镇华

#include"tj_multiset.h"

//交换函数
template<class T>
void _swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

//二叉树搜索树，一直向左走，找到最小的值
template<class T>
node<T>* minimum(node<T>* x) {
	while (x->left != NULL) x = x->left;
	return x;
}

//二叉搜索树，一直向右走，找最大的值
template<class T>
node<T>* maximum(node<T>* x) {
	while (x->right != NULL) x = x->right;
	return x;
}

//左旋
template<class T>
void rotate_left(node<T>* x, node<T>*& root) {
	//x为旋转点
	node<T>* y = x->right;//令y为旋转点的右子节点
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	//令y完全顶替x的地位
	if (x == root)//x为根节点
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

//右旋
template<class T>
void rotate_right(node<T>* x, node<T>*& root) {
	//x为旋转点
	node<T>* y = x->left;//令y为旋转点的左子节点
	x->left = y->right;
	if (y->right != NULL)
		y->right->parent = x;
	y->parent = x->parent;
	//令y完全顶替x的地位
	if (x == root)//x为根节点
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->right = x;
	x->parent = y;
}

//__insert : 内部函数
template<class T, class Compare>
typename tj_rb_tree<T, Compare>::iterator tj_rb_tree<T, Compare>::__insert(node<T>* x_, node<T>* y_, const T& v) {
	//参数x_为新值安插点，参数y_为安插点之父节点，参数v 为新值
	node<T>* x = x_;
	node<T>* y = y_;
	node<T>* z;
	//key_compare是键值得比较准则，是个函数或函数指针
	if (y == header || x != NULL || key_compare(v, y->value)) {
		z = create_node(v);  // 产生一个新节点
		y->left = z;          // 这使得当y为header时，leftmost()=z
		if (y == header) {
			header->parent = z;
			header->right = z;
		}
		else if (y == header->left)	// 如果y为最左节点
			header->left = z;           	// 维护leftmost()，使它永远指向最左节点
	}
	else {
		z = create_node(v);
		y->right = z;				// 令新节点成为安插点之父节点y的右孩子
		if (y == header->right)
			header->right = z;          	// 维护rightmost()，使它永远指向最右节点
	}
	z->parent = y;		// 设定新节点的父节点
	z->left = NULL;		// 设定新孩子节点的左孩子
	z->right = NULL; 		// 设定新孩子节点的右孩子
							// 新节点的颜色将在 __rb_tree_rebalance() 设定并调整
	tj__rb_tree_rebalance(z, header->parent);	// 参数一为新增节点，参数二为root
	++node_count;		// 节点数增加
	return iterator(z);	// 返回迭代器，指向新增节点
}


//插入后重新令rb_tree平衡（改变颜色和旋转）参数x为新增节点，参数二为root节点
template<class T>
inline void tj__rb_tree_rebalance(node<T>* x, node<T>*& root) {
	x->color = _red;		// 新节点比为红色
	while (x != root && x->parent->color == _red) { // 父节点为红色
		if (x->parent == x->parent->parent->left) { // 父节点为祖父节点的左孩子
			node<T>* y = x->parent->parent->right;	// 令y 为伯父节点
			if (y && y->color == _red) { 		// 伯父节点存在，且为红色
				x->parent->color = _black;  		// 更改父节点为黑色
				y->color = _black;				// 更改伯父节点为黑色
				x->parent->parent->color = _red; 	// 更改祖父节点为红色
				x = x->parent->parent;
			}
			else {	// 无伯父节点或伯父节点为黑色（NULL就是黑色）
				if (x == x->parent->right) { // 新增节点为父节点的右孩子
					x = x->parent;
					rotate_left(x, root); // 第一个参数为左旋转点
				}
				x->parent->color = _black;	// 改变颜色，父节点为黑色
				x->parent->parent->color = _red;
				rotate_right(x->parent->parent, root); // 第一参数为右旋转点
			}
		}
		else {	// 父节点为祖父节点的右孩子
			node<T>* y = x->parent->parent->left; // y为伯父节点
			if (y && y->color == _red) {		// 有伯父节点且为红色
				x->parent->color = _black;		// 更改父节点为黑色
				y->color = _black; 				// 更改伯父节点为黑色
				x->parent->parent->color = _red; 	// 更改祖父节点为红色
				x = x->parent->parent;	// 准备继续往上层检查……
			}
			else {	// 无伯父节点或伯父节点为黑色（NULL就是黑色）
				if (x == x->parent->left) {	// 新节点为父节点的左孩子
					x = x->parent;
					rotate_right(x, root); 	// 第一个参数右旋转
				}
				x->parent->color = _black;	// 改变颜色，父节点为黑色
				x->parent->parent->color = _red;
				rotate_left(x->parent->parent, root); // 第一个参数做旋转
			}
		}
	}	// while 結束
	root->color = _black;	// 根节点永远为黑色
}

//删除后重新令rb_tree平衡
template<class T>
inline node<T>* tj__rb_tree_rebalance_for_erase(node<T>* z, node<T>*& root,
	node<T>*& leftmost, node<T>*& rightmost)
{
	node<T>* y = z;
	node<T>* x = NULL;
	node<T>* x_parent = NULL;
	if (y->left == NULL)             // z has at most one non-null child. y == z.
		x = y->right;               // x might be null.
	else {
		if (y->right == NULL)          // z has exactly one non-null child.  y == z.
			x = y->left;              // x is not null.
		else {                      // z has two non-null children.  Set y to
			y = y->right;             //   z's successor.  x might be null.
			while (y->left != NULL)
				y = y->left;
			x = y->right;
		}
	}
	if (y != z) {                 // relink y in place of z.  y is z's successor
		z->left->parent = y;
		y->left = z->left;
		if (y != z->right) {
			x_parent = y->parent;
			if (x) 
				x->parent = y->parent;
			y->parent->left = x;      // y must be a left child
			y->right = z->right;
			z->right->parent = y;
		}
		else
			x_parent = y;
		if (root == z)
			root = y;
		else if (z->parent->left == z)
			z->parent->left = y;
		else
			z->parent->right = y;
		y->parent = z->parent;
		_swap(y->color, z->color);
		y = z;
		// y now points to node to be actually deleted
	}
	else {                        // y == z
		x_parent = y->parent;
		if (x) 
			x->parent = y->parent;
		if (root == z)
			root = x;
		else
			if (z->parent->left == z)
				z->parent->left = x;
			else
				z->parent->right = x;
		if (leftmost == z)
			if (z->right == NULL)        // z->left must be null also
				leftmost = z->parent;
		// makes leftmost == header if z == root
			else
				leftmost = minimum(x);
		if (rightmost == z)
			if (z->left == NULL)         // z->right must be null also
				rightmost = z->parent;
		// makes rightmost == header if z == root
			else                      // x == z->left
				rightmost = maximum(x);
	}
	if (y->color != _red) {
		while (x != root && (x == NULL || x->color == _black))
			if (x == x_parent->left) {
				node<T>* w = x_parent->right;
				if (w->color == _red) {
					w->color = _black;
					x_parent->color = _red;
					rotate_left(x_parent, root);
					w = x_parent->right;
				}
				if ((w->left == NULL || w->left->color == _black) &&
					(w->right == NULL || w->right->color == _black)) {
					w->color = _red;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else {
					if (w->right == NULL || w->right->color == _black) {
						if (w->left) w->left->color = _black;
						w->color = _red;
						rotate_right(w, root);
						w = x_parent->right;
					}
					w->color = x_parent->color;
					x_parent->color = _black;
					if (w->right) w->right->color = _black;
					rotate_left(x_parent, root);
					break;
				}
			}
			else {                  // same as above, with right <-> left.
				node<T>* w = x_parent->left;
				if (w->color == _red) {
					w->color = _black;
					x_parent->color = _red;
					rotate_right(x_parent, root);
					w = x_parent->left;
				}
				if ((w->right == NULL || w->right->color == _black) &&
					(w->left == NULL || w->left->color == _black)) {
					w->color = _red;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else {
					if (w->left == NULL || w->left->color == _black) {
						if (w->right) w->right->color = _black;
						w->color = _red;
						rotate_left(w, root);
						w = x_parent->left;
					}
					w->color = x_parent->color;
					x_parent->color = _black;
					if (w->left) w->left->color = _black;
					rotate_right(x_parent, root);
					break;
				}
			}
		if (x) x->color = _black;
	}
	return y;
}

//查找函数
template<class T, class Compare>
typename tj_rb_tree<T, Compare>::iterator tj_rb_tree<T, Compare>::find(const T& k) {
	node<T>* y = header;        // Last node which is not less than k. 
	node<T>* x = header->parent;        // Current node. 

	while (x != NULL)
		// key_compare 是 function object。
		if (!key_compare(x->value, k))
			// 运行到这里，表示x键值大于k。遇到大值就向左走。
			y = x, x = x->left;	// 注意语法!逗号表达式
		else
			// 运行到这里，表示x键值小于k。遇到小值就向右走。
			x = x->right;

	iterator j = iterator(y);
	return (j == end() || key_compare(k, j.ptr->value)) ? end() : j;
}

//插入函数 允许键值重复。返回新插入节点的迭代器
template <class T, class Compare>
typename tj_rb_tree<T, Compare>::iterator tj_rb_tree<T, Compare>::insert_equal(const T& v)
{
	node<T>* y = header;
	node<T>* x = header->parent;
	while (x != NULL) {		// 从根节点开始，向下寻找适当安插位置
		y = x;
		x = key_compare(v, x->value) ? x->left : x->right;
	}
	return __insert(x, y, v);
}

//插入函数 不允许键值重复，否则安插无效。返回值是个pair，第一个元素是个RB-tree迭代器，指向新增节点。第二个元素表示安插是否成功。
template <class T, class Compare> 
pair<typename tj_rb_tree<T, Compare>::iterator, bool>
tj_rb_tree<T, Compare>::insert_unique(const T& v)
{
	node<T>* y = header;
	node<T>* x = header->parent;  //从根节点开始
	bool comp = true;
	while (x != NULL) { 		// 从根节点开始向下寻找适当安插位置
		y = x;
		comp = key_compare(v, x->value); // v 键值小于目前节点的键值？
		x = comp ? x->left : x->right;	// 遇「大」往左，遇「小于或等于」往右
	}
	//离开while循环之后，y所指即为安插点的父节点，x必为叶子节点

	iterator j = iterator(y);   // 令迭代器j指向安插点之父节点 y
	if (comp)	//如果离开while循环时comp为真，表示 父节点键值>v ，将安插在左孩子处
		if (j == begin())   // 如果j是最左节点
			return pair<iterator, bool>(__insert(x, y, v), true);
	// 以上，x 为安插点，y 为安插点之父节点，v 为新值。
		else	// 否则（安插点之父节点不是最左节点）
			--j;	// 调整 j，回头准备测试...
	if (key_compare(j.ptr->value, v))
		// 小于新值（表示遇「小」，将安插于右侧）
		return pair<iterator, bool>(__insert(x, y, v), true);

	//若运行到这里，表示键值有重复，不应该插入
	return pair<iterator, bool>(j, false);
}

//删除函数
template <class T, class Compare>
typename tj_rb_tree<T, Compare>::iterator tj_rb_tree<T, Compare>::erase(iterator position) {
	iterator origin = position++;
	node<T>* y = tj__rb_tree_rebalance_for_erase(origin.ptr,
		header->parent,
		header->left,
		header->right);
	destroy_node(y);
	--node_count;
	return position;
}

//删除函数
template <class T, class Compare>
size_t tj_rb_tree<T, Compare>::erase(const T& x) {
	size_t n = 0;
	pair<iterator, iterator> p = equal_range(x);
	distance(p.first, p.second, n);
	erase(p.first, p.second);
	return n;
}

//删除函数
template <class T, class Compare>
inline void tj_rb_tree<T, Compare>::erase(iterator first, iterator last) {
	if (first == begin() && last == end())
		clear();
	else
		while (first != last) 
			erase(first++);
}

//距离函数
template <class T, class Compare>
inline void tj_rb_tree<T, Compare>::distance(iterator first, iterator last, size_t& n) {
	while (first != last) {
		first++;
		n++;
	}
}

//查找第一个不比k小的节点
template <class T, class Compare>
typename tj_rb_tree<T, Compare>::iterator
tj_rb_tree<T, Compare>::lower_bound(const T& k) {
	node<T>* y = header; //第一个不比k小的节点
	node<T>* x = header->parent;//当前节点

	while (x != NULL)
		if (!key_compare(x->value, k))
			y = x, x = x->left;
		else
			x = x->right;
	return iterator(y);
}

//查找第一个比k大的节点
template <class T, class Compare>
typename tj_rb_tree<T, Compare>::iterator
tj_rb_tree<T, Compare>::upper_bound(const T& k) {
	node<T>* y = header;//第一个比k大的节点
	node<T>* x = header->parent;//当前节点

	while (x != NULL)
		if (key_compare(k, x->value))
			y = x, x = x->left;
		else
			x = x->right;
	return iterator(y);
}

//返回相等元素的范围
template <class T, class Compare>
pair<typename tj_rb_tree<T, Compare>::iterator, typename tj_rb_tree<T, Compare>::iterator>
tj_rb_tree<T, Compare>::equal_range(const T& k) {
	return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

//计算从node至root路径中的黑节点数量
template<class T>
inline int tj__black_count(node<T>* n, node<T>* root)
{
	if (n == NULL)
		return 0;
	else {
		int bc = n->color == _black ? 1 : 0;
		if (n == root)
			return bc;
		else
			return bc + tj__black_count(n->parent, root);//累加
	}
}

//检验是否是红黑树
template <class T, class Compare> 
bool tj_rb_tree<T, Compare>::tj__rb_verify() {
	// 空树，符合RB树标准
	if (node_count == 0 || begin() == end())
		return node_count == 0 && begin() == end() && header->left == header && header->right == header;

	//最左（叶）节点至 root 路径的黑节点个数
	int len = tj__black_count(header->left, header->parent);
	//一下走访整个RB树，针对每个节点（从最小到最大）……
	for (iterator it = begin(); it != end(); ++it) {
		node<T>* x = it.ptr; // __rb_tree_base_iterator::node
		node<T>* L = x->left;		// 这是左子节点
		node<T>* R = x->right; 	// 这是右子节点

		if (x->color == _red)
			if ((L && L->color == _red) ||
				(R && R->color == _red))
				return false;	// 父子节点同为红色，不合符RB树要求

		if (L && key_compare(x->value, L->value)) // 当前节点的键值小于左孩子节点的键值
			return false;         	// 不符合二叉查找树的要求
		if (R && key_compare(R->value, x->value)) // 当前节点的键值大于右孩子节点的键值
			return false;		// 不符合二叉查找树的要求

		  //[叶子结点到root]路径内的黑色节点数，与[最左节点至root]路径内的黑色节点不同。不符合RB树要求
		if (!L && !R && tj__black_count(x, header->parent) != len)
			return false;
	}

	if (header->left != minimum(header->parent))
		return false;	// 最左节点不为最小节点，不符合二叉查找树的要求。
	if (header->right != maximum(header->parent))
		return false;	// 最右节点不为最大节点，不符不符合二叉查找树的要求。
	return true;
}

//重载 == 运算符
template <class T, class Compare>
bool tj_rb_tree<T, Compare>::operator == (const tj_rb_tree<T, Compare>& x)
{
	if (size() != x.size())
		return false;
	auto it1 = begin();
	auto it2 = x.begin();
	for ( ;it1 != end() && it2 != x.end();it1++, it2++)
		if (*it1 != *it2)
			return false;
	return true;
}

//重载 != 运算符
template <class T, class Compare>
bool tj_rb_tree<T, Compare>::operator != (const tj_rb_tree<T, Compare>& x)
{
	return !(*this == x);
}

//重载 < 运算符
template <class T, class Compare>
bool tj_rb_tree<T, Compare>::operator < (const tj_rb_tree<T, Compare>& x)
{
	auto it1 = begin();
	auto it2 = x.begin();
	for (; it1 != end() && it2 != x.end(); it1++, it2++)
		if (*it1 != *it2)
			return *it1 < *it2;
	return size() < x.size();
}

//重载 <= 运算符
template <class T, class Compare>
bool tj_rb_tree<T, Compare>::operator <= (const tj_rb_tree<T, Compare>& x)
{
	auto it1 = begin();
	auto it2 = x.begin();
	for (; it1 != end() && it2 != x.end(); it1++, it2++)
		if (*it1 != *it2)
			return *it1 < *it2;
	return size() <= x.size();
}

//重载 > 运算符
template <class T, class Compare>
bool tj_rb_tree<T, Compare>::operator > (const tj_rb_tree<T, Compare>& x)
{
	auto it1 = begin();
	auto it2 = x.begin();
	for (; it1 != end() && it2 != x.end(); it1++, it2++)
		if (*it1 != *it2)
			return *it1 > *it2;
	return size() > x.size();
}

//重载 >= 运算符
template <class T, class Compare>
bool tj_rb_tree<T, Compare>::operator >= (const tj_rb_tree<T, Compare>& x)
{
	auto it1 = begin();
	auto it2 = x.begin();
	for (; it1 != end() && it2 != x.end(); it1++, it2++)
		if (*it1 != *it2)
			return *it1 > *it2;
	return size() >= x.size();
}