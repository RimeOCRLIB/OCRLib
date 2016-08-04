
#include <vector>
#include <algorithm>

//https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Расстояние_Левенштейна


template <typename T>
typename T::size_type levenshteinDistance(const T & src, const T & dst) {
    const typename T::size_type m = src.size();
    const typename T::size_type n = dst.size();
    if (m == 0) {
        return n;
    }
    if (n == 0) {
        return m;
    }
    
    std::vector< std::vector<typename T::size_type> > matrix(m + 1);
    
    for (typename T::size_type i = 0; i <= m; ++i) {
        matrix[i].resize(n + 1);
        matrix[i][0] = i;
    }
    for (typename T::size_type i = 0; i <= n; ++i) {
        matrix[0][i] = i;
    }
    
    typename T::size_type above_cell, left_cell, diagonal_cell, cost;
    
    for (typename T::size_type i = 1; i <= m; ++i) {
        for(typename T::size_type j = 1; j <= n; ++j) {
            cost = src[i - 1] == dst[j - 1] ? 0 : 1;
            above_cell = matrix[i - 1][j];
            left_cell = matrix[i][j - 1];
            diagonal_cell = matrix[i - 1][j - 1];
            matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
        }
    }
    
    return matrix[m][n];
}