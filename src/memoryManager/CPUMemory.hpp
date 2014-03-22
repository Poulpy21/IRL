
#ifndef __CPU_MIN_RESERVED_MEMORY
#define __CPU_MIN_RESERVED_MEMORY 2000000000
#endif

#include <ostream>

#ifndef CPUMEMORY_H
#define CPUMEMORY_H

class CPUMemory {
	public:
		static void init();

		static unsigned long memorySize(); //bytes
		static unsigned long memoryLeft(); //bytes

		static bool canAllocateBytes(unsigned int nBytes);

		template <typename T>
		static bool canAllocate(unsigned int nData);

		template <typename T>
		static T *malloc(unsigned int nData, bool pinnedMemory=false);

		template <typename T>
		static void free(T *data, unsigned int nData, bool force=false);

		static void display(std::ostream &out);
			

	private:
		CPUMemory();
		static const volatile unsigned long _memorySize;
		static const volatile unsigned long _memoryRuntime;
		static unsigned long _memoryLeft;
};

#include "CPUMemory.tpp"

#endif /* end of include guard: CPUMEMORY_H */
