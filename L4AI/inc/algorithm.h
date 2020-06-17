#ifndef ALGORITHMSPECIFIER_H
#define ALGORITHMSPECIFIER_H
#include <stddef.h>
#include <L4AI_global.h>

namespace l4ai::algs {

	/**
	 * @brief
	 */
	enum class AlgorithmType {
		Unknown, Perceptron, Pipe
	};


	/**
	 * @brief Данный класс описывает спецификацию некоторого абстрактного алгоритма, о котором известно лишь то, что у него есть входные и выходные данные.
	 *
	 *
	 *       _______
	 * В -->|		|-->В  <br/>
	 * х -->| Алго- |-->ы  <br/>
	 * о -->| ритм  |-->х  <br/>
	 * д -->|		|-->о  <br/>
	 * ы -->|		|-->д  <br/>
	 *      |_______|-->ы  <br/>
	 */
	class L4AI_EXPORT Algorithm {
	private:
		size_t input_length;
		size_t output_length;
	protected:
		AlgorithmType alg_type;
	public:
		Algorithm();
		Algorithm ( size_t input_length, size_t output_length );

		size_t getInputLength() const;
		void setInputLength ( const size_t& value );
		void setInputLength ( size_t&& value );
		size_t getOutputLength() const;
		void setOutputLength ( const size_t& value );
		void setOutputLength ( size_t&& value );

		template<class TTarget>
		constexpr inline TTarget* as_ptr() { return static_cast<TTarget*>(this); }
		template<class TTarget>
		constexpr inline const TTarget* as_ptr() const { return static_cast<const TTarget*>(this); }
		template<class TTarget>
		constexpr inline TTarget& as() { return *as_ptr<TTarget>(); }
		template<class TTarget>
		constexpr inline const TTarget& as() const { return *as_ptr<TTarget>(); }

		inline AlgorithmType type() const { return alg_type; }
	};

} // l4ai::algs

#endif // ALGORITHMSPECIFIER_H
