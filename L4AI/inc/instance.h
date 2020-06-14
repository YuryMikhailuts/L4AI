#ifndef INSTANCE_H
#define INSTANCE_H
#include <algorithm.h>
#include <memory>

namespace l4ai::algs {

	template<typename TValue>
	class Instance {
	public:
		using value_t = TValue;
	private:
		std::unique_ptr<Algorithm> algorithm;
	protected:
		Instance ( std::unique_ptr<Algorithm>&& algorithm );
		Instance ( Algorithm*&& algorithm );
	public:
		const Algorithm& getAlgorithm() const;
	};

	using InstanceF32 = Instance<float>;
	using InstanceF64 = Instance<double>;
}

#endif // INSTANCE_H
