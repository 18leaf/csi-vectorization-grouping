//
// Created by Lindsey on 2/15/2025.
//

#include "../include/vectorizer.h"
#include <vector>
#include <string>

class DummyVectorizer : public Vectorizer {
public:
	std::vector<float> vectorize(const std::string& text) override {
		// only size of message
		return std::vector<float>(text.size(), 1.0f);
	}
};


Vectorizer* createVectorizer() {
	return new DummyVectorizer();
}