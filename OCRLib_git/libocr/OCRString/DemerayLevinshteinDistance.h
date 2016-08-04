//
//  DemerayLevinshteinDistance.h
//  OCRLib
//
//  Created by dharmabook on 24/05/15.
//
//


#include <algorithm>

#ifndef OCRLib_DemerayLevinshteinDistance_h
#define OCRLib_DemerayLevinshteinDistance_h

/// <summary>
/// Damerau-Levenshtein distance
//http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C.2B.2B
/// </summary>

#include <algorithm>



namespace ocr{

    
    
    class DamerauLevensteinMetric {
    private:
        const int DEFAULT_LENGTH = 255;
        int *_currentRow;
        int *_previousRow;
        int *_transpositionRow;
        int maxLength;
    public:
        DamerauLevensteinMetric (void){}
        
        /// <summary>
        ///
        /// </summary>
        /// <param name="maxLength"></param>
        DamerauLevensteinMetric(int maxLength_)
        {
            _currentRow = new int[maxLength_ + 1];
            _previousRow = new int[maxLength_ + 1];
            _transpositionRow = new int[maxLength_ + 1];
            maxLength=maxLength_+1;
        }
        
        /// <summary>
        /// Damerau-Levenshtein distance is computed in asymptotic time O((max + 1) * min(first.length(), second.length()))
        /// </summary>
        /// <param name="first"></param>
        /// <param name="second"></param>
        /// <param name="max"></param>
        /// <returns></returns>
        int GetDistance(string first, string second, int max){
            int firstLength = (uint)first.size();
            int secondLength = (uint)second.size();
            
            if (firstLength == 0)
                return secondLength;
            
            if (secondLength == 0) return firstLength;
            
            if (firstLength > secondLength)
            {
                string tmp = first;
                first = second;
                second = tmp;
                firstLength = secondLength;
                secondLength = (uint)second.size();
            }
            
            if (max < 0) max = secondLength;
            if (secondLength - firstLength > max) return max + 1;
            
            if (firstLength > maxLength)
            {
                _currentRow = new int[firstLength + 1];
                _previousRow = new int[firstLength + 1];
                _transpositionRow = new int[firstLength + 1];
            }
            
            for (int i = 0; i <= firstLength; i++)
                _previousRow[i] = i;
            
            char lastSecondCh = (char) 0;
            for (int i = 1; i <= secondLength; i++)
            {
                char secondCh = second[i - 1];
                _currentRow[0] = i;
                
                // Compute only diagonal stripe of width 2 * (max + 1)
                int from = std::max(i - max - 1, 1);
                int to = std::min(i + max + 1, firstLength);
                
                char lastFirstCh = (char) 0;
                for (int j = from; j <= to; j++)
                {
                    char firstCh = first[j - 1];
                    
                    // Compute minimal cost of state change to current state from previous states of deletion, insertion and swapping
                    int cost = firstCh == secondCh ? 0 : 1;
                    int value = std::min(std::min(_currentRow[j - 1] + 1, _previousRow[j] + 1), _previousRow[j - 1] + cost);
                    
                    // If there was transposition, take in account its cost
                    if (firstCh == lastSecondCh && secondCh == lastFirstCh)
                        value = std::min(value, _transpositionRow[j - 2] + cost);
                    
                    _currentRow[j] = value;
                    lastFirstCh = firstCh;
                }
                lastSecondCh = secondCh;
                
                int *tempRow = _transpositionRow;
                _transpositionRow = _previousRow;
                _previousRow = _currentRow;
                _currentRow = tempRow;
            }
            
            return _previousRow[firstLength];
        }
    };
};

#endif
