
#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <vector>
#include <math.h>

namespace fefu
{

    template<typename T>
    class allocator {
    public:
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = typename std::add_lvalue_reference<T>::type;
        using const_reference = typename std::add_lvalue_reference<const T>::type;
        using value_type = T;

        allocator() noexcept = default;

        allocator(const allocator&) noexcept = default;

        template <class U>
        allocator(const allocator<U>&) noexcept {

        }

        ~allocator() {

        }

        pointer allocate(size_type size) {
            auto p = ::operator new (size * sizeof(T));
            return static_cast<pointer>(p);
        }

        void deallocate(pointer p, size_type n) noexcept {
            ::operator delete(p, n * sizeof(value_type));
        }
    };

    template<typename ValueType>
    class hash_map_iterator {

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using reference = ValueType&;
        using pointer = ValueType*;

        hash_map_iterator() noexcept {}
        hash_map_iterator(const hash_map_iterator& other) noexcept
        : idx_(other.idx_), m_s(other.m_s), m_d(other.m_d) {}

        reference operator*() const{
            return *(m_d + idx_);
        }
        pointer operator->() const{
            return (m_d + idx_);
        }

        // prefix ++
        hash_map_iterator& operator++(){
            idx_++;
            while(idx_ < m_s->size() && *(m_s->data() +idx_) != 1){
                idx_++;
            }
            return *this;
        }
        // postfix ++
        hash_map_iterator operator++(int){
            hash_map_iterator other(*this);
            idx_++;
            while (idx_ < m_s->size() && *(m_s->data() +idx_) != 1){
                idx_++;
            }
            return other;
        }
        template<typename, typename, typename, typename, typename>
        friend class hash_map;
        template<typename>
        friend class hash_map_const_iterator;
        friend bool operator==(const hash_map_iterator<ValueType>& a, const hash_map_iterator<ValueType>& b){
            return (*(a.m_s) == *(b.m_s) && a.m_d == b.m_d && a.idx_ == b.idx_);
        }
        friend bool operator!=(const hash_map_iterator<ValueType>& a, const hash_map_iterator<ValueType>& b){
            return !(*(a.m_s) == *(b.m_s) && a.m_d == b.m_d && a.idx_ == b.idx_);
        }
    private:
        explicit hash_map_iterator(std::vector<char>* m_s_, value_type* m_d_, std::size_t idx)
            : m_s(m_s_), m_d(m_d_), idx_(idx) {}
        hash_map_iterator(std::vector<char>* m_s_, value_type* m_d_)
                : m_s(m_s_), m_d(m_d_) {
            idx_++;
            while (idx_ < m_s->size() && *(m_s->data() +idx_) != 1){
                idx_++;
            }
        }
        std::size_t idx_ = 0;
        std::vector<char>* m_s;
        value_type* m_d;
    };

    template<typename ValueType>
    class hash_map_const_iterator {
// Shouldn't give non const references on value
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using reference = const ValueType&;
        using pointer = const ValueType*;

        hash_map_const_iterator() noexcept {}
        hash_map_const_iterator(const hash_map_const_iterator& other) noexcept
            : idx_(other.idx_), m_s(other.m_s), m_d(other.m_d) {}
        hash_map_const_iterator(const hash_map_iterator<ValueType>& other) noexcept
               : idx_(other.idx_), m_s(other.m_s), m_d(other.m_d) {}

        reference operator*() const {
            return *(m_d + idx_);
        }
        pointer operator->() const{
            return (m_d + idx_);
        }

        // prefix ++
        hash_map_const_iterator& operator++(){
            idx_++;
            while (idx_ < m_s->size() && *(m_s->data() +idx_) != 1){
                idx_++;
            }
            return *this;
        }
        // postfix ++
        hash_map_const_iterator operator++(int){
            hash_map_const_iterator other(*this);
            idx_++;
            while (idx_ < m_s->size() && *(m_s->data() + idx_) != 1){
                idx_++;
            }
            return other;
        }
        template<typename, typename, typename, typename, typename>
        friend class hash_map;

        friend bool operator==(const hash_map_const_iterator<ValueType>& a, const hash_map_const_iterator<ValueType>& b){
            return (a.m_s == b.m_s && a.m_d == b.m_d && a.idx_ == b.idx_);
        }
        friend bool operator!=(const hash_map_const_iterator<ValueType>& a, const hash_map_const_iterator<ValueType>& b){
            return !(a.m_s == b.m_s && a.m_d == b.m_d && a.idx_ == b.idx_);
        }
    private:
       explicit hash_map_const_iterator(const std::vector<char>* m_s_, value_type* m_d_, std::size_t idx)
                : m_s(m_s_), m_d(m_d_), idx_(idx) {}
        hash_map_const_iterator(const std::vector<char>* m_s_, value_type* m_d_)
                :m_s(m_s_), m_d(m_d_){
            idx_++;
            while (idx_ < m_s->size() && (*(m_s->data() +idx_) != 1)){
                idx_++;
            }
        }
        std::size_t idx_ = 0;
        const std::vector<char>* m_s;
        value_type* m_d;
    };
    template<typename K, typename T,
            typename Hash = std::hash<K>,
            typename Pred = std::equal_to<K>,
            typename Alloc = allocator<std::pair<const K, T>>>
    class hash_map
    {
    public:
        using key_type = K;
        using mapped_type = T;
        using hasher = Hash;
        using key_equal = Pred;
        using allocator_type = Alloc;
        using value_type = std::pair<const key_type, mapped_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = hash_map_iterator<value_type>;
        using const_iterator = hash_map_const_iterator<value_type>;
        using size_type = std::size_t;

        hash_map(): m_set(1, 0),
                    m_data(m_alloc.allocate(1)){}

        explicit hash_map(size_type n) :
                m_set(n, 0),
                m_data(m_alloc.allocate(n)){}

        template<typename InputIterator>
        hash_map(InputIterator first, InputIterator last,
                 size_type n = 0): hash_map(){
            this->insert(first, last);
        }

        hash_map(const hash_map& other):
            m_alloc(other.m_alloc),
            m_hash(other.m_hash),
            m_key_equal(other.m_key_equal),
            _max_load_factor(other._max_load_factor),
            m_set(other.bucket_count(), 0),
            m_data(m_alloc.allocate(other.bucket_count())){
            this->insert(other.begin(), other.end());
        }

        hash_map(hash_map&& other): m_alloc(std::move(other.m_alloc)),
                    m_hash(std::move(other.m_hash)),
                    m_key_equal(std::move(other.m_key_equal)),
                    _max_load_factor(std::move(other._max_load_factor)),
                    m_set(std::move(other.m_set)),
                    m_data(std::move(other.m_data)){
            other.m_data = nullptr;
        }

        explicit hash_map(const allocator_type& a):m_alloc(a) { hash_map(); }

        hash_map(const hash_map& umap,
                 const allocator_type& a): m_alloc(a),
                                        m_hash(umap.m_hash),
                                           m_key_equal(umap.m_key_equal),
                                           _max_load_factor(umap._max_load_factor),
                                           m_set(umap.bucket_count(), 0),
                                           m_data(m_alloc.allocate(umap.bucket_count())){
            this->insert(umap.begin(), umap.end());
        }

        hash_map(hash_map&& umap,
                 const allocator_type& a): m_alloc(a),
        m_hash(std::move(umap.m_hash)),
        m_key_equal(std::move(umap.m_key_equal)),
        _max_load_factor(std::move(umap._max_load_factor)),
        m_set(std::move(umap.m_set)),
        m_data(std::move(umap.m_data)){
            umap.m_data = nullptr;
        }

        hash_map(std::initializer_list<value_type> l,
                 size_type n = 0): m_set(l.size(), 0),
                                   m_data(m_alloc.allocate(l.size())){
            this->insert(l);
        }

        ~hash_map(){
            m_alloc.deallocate(m_data, size_type(m_set.size()));
        }
        hash_map& operator=(const hash_map& other){
            hash_map(other).swap(*this);
            return *this;
        }

        hash_map& operator=(hash_map&&) = default;

        hash_map& operator=(std::initializer_list<value_type> l){
            hash_map a(l);
            a.swap(*this);
            return *this;
        }

        allocator_type getAllocator() const noexcept { return m_alloc; }


        bool empty() const noexcept { return size() == 0; }

        size_type size() const noexcept {
            return m_size;
        }

        size_type max_size() noexcept {
            return std::numeric_limits<size_type>::max();
        }


        iterator begin() noexcept { return iterator(&m_set, m_data); }

        const_iterator begin() const noexcept { return const_iterator(&m_set, m_data); }

        const_iterator cbegin() const noexcept { return const_iterator(&m_set, m_data); }

        iterator end() noexcept {return iterator(&m_set, m_data, m_set.size());}

        const_iterator end() const noexcept { return const_iterator(&m_set, m_data, m_set.size()); }

        const_iterator cend() const noexcept { return const_iterator(&m_set, m_data, m_set.size()); }


        template<typename... _Args>
        std::pair<iterator, bool> emplace(_Args&&... args){
            return emplace_impl(std::forward<_Args>(args)...);
        }

        template <typename... _Args>
        std::pair<iterator, bool> try_emplace(const key_type& k, _Args&&... args){
            return emplace_impl(k, mapped_type(std::forward<_Args>(args)...));
        }

        template <typename... _Args>
        std::pair<iterator, bool> try_emplace(key_type&& k, _Args&&... args){
            return emplace_impl(std::move(k), mapped_type(std::forward<_Args>(args)...));
        }

        std::pair<iterator, bool> insert(const value_type& x) {
            return emplace_impl(x.first, x.second);
        }


        std::pair<iterator, bool> insert(value_type&& x){
            return emplace_impl(x.first, std::move(x.second));

        }

        template<typename _InputIterator>
        void insert(_InputIterator first, _InputIterator last){
            for (; first != last; ++first)
            {
                insert(*first);
            }
        }

        void insert(std::initializer_list<value_type> l){
            insert(l.begin(), l.end());
        }


        template <typename _Obj>
        std::pair<iterator, bool> insert_or_assign(const key_type& k, _Obj&& obj){
            std::pair<iterator, bool> i = this->insert(value_type{k, std::forward<_Obj>(obj)});
            if (!i.second){
                (*i.first).second = mapped_type(obj);
            }
            return i;
        }

        template <typename _Obj>
        std::pair<iterator, bool> insert_or_assign(key_type&& k, _Obj&& obj){
            std::pair<iterator, bool> i = this->insert(value_type{std::move(k), mapped_type(std::forward<_Obj>(obj))});
            if (!i.second){
                (*i.first).second = mapped_type(obj);
            }
            return i;
        }

        iterator erase(const_iterator position){
            m_set[position.idx_] = 2;
            ++position;
            return iterator(&m_set, m_data, position.idx_);
        }

        iterator erase(iterator position){
            m_set[position.idx_] = 2;
            return ++position;
        }

        size_type erase(const key_type& x) {
            auto it = find_impl(x);
            if (it != end()) {
                m_set[it.idx_] = 2;
                return 1;
            }
            return 0;
        }
//        iterator erase(const_iterator first, const_iterator last){
//            auto y(last);
//            ++y;
//            for (; first != last; ) {
//                m_set[i.idx_] = 2;
//            }
//            return iterator(m_set, m_data, last.idx_);
//        }

        void clear() noexcept{
            hash_map x (bucket_count());
            swap(x);
        }

        void swap(hash_map& x){
            std::swap(m_set, x.m_set);
            std::swap(m_data, x.m_data);
            std::swap(m_size, x.m_size);
            std::swap(_max_load_factor, x._max_load_factor);
        }

        template<typename _H2, typename _P2>
        void merge(hash_map<K, T, _H2, _P2, Alloc>& source){
            for (auto i = source.begin(); i != source.end(); ++i){
                insert(*i);
            }
        }

        Hash hash_function() const { return m_hash; }

        Pred key_eq() const { return m_key_equal; }

        iterator find(const key_type& x) { return find_impl(x); }

        const_iterator find(const key_type& x) const{
            return const_find_impl(x);
        }

        size_type count(const key_type& x) const {

            auto a = (find(x) == cend() ? 0 : 1);
            return a;
        }

        bool contains(const key_type& x) const{
            return !(find(x) == cend());
        }

        mapped_type& operator[](const key_type& k){
            return emplace_impl(k).first->second;
        }


        mapped_type& operator[](key_type&& k){
            return emplace_impl(std::move(k)).first->second;
        }

        mapped_type& at(const key_type& k) { return at_impl(k); }

        const mapped_type& at(const key_type& k) const {return at_impl(k);}

        size_type bucket_count() const noexcept{
            return m_set.size();
        }

        size_type bucket(const key_type& _K) const{
            const size_type h = m_hash(_K);
            const size_type index = h % m_set.size();
            return index;
        }

        float load_factor() const noexcept{
            return float(float(size()) / float(bucket_count()));
        }

        float max_load_factor() const noexcept{
            return _max_load_factor;
        }

        void max_load_factor(float z){
            if (z < max_load_factor()) {
                _max_load_factor = z;
                reserve(size());
            }
            _max_load_factor = z;
        }

        void rehash(size_type n){
            if (n <= m_set.size()){
                return;
            }
            hash_map other(n);
            for (auto i = (*this).begin(); i != (*this).end(); ++i) {
                other.insert(*i);
            }
            swap(other);
        }

        void reserve(size_type n){
            rehash(ceil(n / max_load_factor()));
        }
        bool operator==(const hash_map& other) const{
            if (!(other.size() == this->size())){
                return false;
            }
            for (size_type i = 0; i < this->bucket_count(); i++){
                if (m_set[i] == 1){
                    if (!(other.contains(m_data[i].first) && at(m_data[i].first) == other.at(m_data[i].first))){
                        return false;
                    }
                }
            }
            return true;
        }

    private:
        allocator_type m_alloc;
        hasher m_hash;
        size_type m_size = 0;
        key_equal m_key_equal;
        std::vector<char> m_set;
        value_type* m_data;
        float _max_load_factor = 0.75 ;
        template <typename A, typename... Args>
        std::pair<iterator, bool> emplace_impl(const A &key, Args &&... args) {
            reserve(m_size + 1);
            const size_type h = m_hash(key);
            const size_type index = h % m_set.size();
            for (size_t a = index ;; (a + 1) % m_set.size()) {
                if (key_equal()(m_set[a], 0)) {
                    new(m_data + a) value_type(key, mapped_type(std::forward<Args>(args)...));
                    m_set[a] = 1;
                    m_size++;
                    m_data[a].second = mapped_type(std::forward<Args>(args)...);
                    return {iterator(&m_set, m_data, a), true};
                } else if (key_equal()(m_set[a], 2)) {
                    m_set[a] = 1;
                    m_data[a].second = mapped_type(std::forward<Args>(args)...);
                    return {iterator(&m_set, m_data, a), true};
                } else if (key_equal()( m_data[a].first, key)) {
                    return {iterator(&m_set, m_data, a), false};
                }
            }
        }
        template <typename A > iterator find_impl(const A &key) {
            const size_type h = m_hash(key);
            const size_type index = h % m_set.size();
            for (size_t a = index ;; (a + 1) % m_set.size()) {
                auto t = m_data[a].first;
                if (key_equal()(m_data[a].first, key) && m_set[a] == 1){
                    return (iterator(&m_set, m_data, a));
                }
                if (key_equal()(m_set[a], 0) || key_equal()(m_set[a], 2)){
                    return end();
                }
            }
          }
        template <typename A> const_iterator const_find_impl(const A &key) const {
            const size_type h = m_hash(key);
            const size_type index = h % m_set.size();
            for (size_t a = index ;; (a + 1) % m_set.size()) {
                if (key_equal()(m_data[a].first, key) && m_set[a] == 1){
                    return (const_iterator(&m_set, m_data, a));
                }
                if (key_equal()(m_set[a], 0) || key_equal()(m_set[a], 2)){
                    return cend();
                }
            }
        }
        template <typename A> mapped_type &at_impl(const A &key) {
            iterator it = find_impl(key);
            if (it != end()) {
                return it->second;
            }
            else {
                throw std::out_of_range("hash_map::at");
            }
        }

        template <typename A> const mapped_type &at_impl(const A &key) const {
            return const_cast<hash_map *>(this)->at_impl(key);
        }

    };
}
