//
// Created by Lindsey on 2/15/2025.
//

#ifndef VECTORIZER_H
#define VECTORIZER_H

#include <string>
#include <vector>

class Vectorizer {
public:
	virtual ~Vectorizer() = default;
	// text message to vector encoding
	virtual std::vector<float> vectorize(const std::string& text) = 0;
};

#endif //VECTORIZER_H


