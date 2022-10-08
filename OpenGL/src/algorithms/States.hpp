#pragma once

namespace States
{
	// check state
	template<typename T>
	bool IsActive(T* states, int target)
	{
		/*
		 * example
		 * states = 10001101
		 * target = 3
		 * 1 = 00000001
		 * 1 << target = 00001000
		 *
		 * 10001101 &
		 * 00001000 & 
		 * -------- returns 1 where both are 1
		 * 00001000 == 1 << target
		 */
		return ((*states) & (1 << target));
	}

	template<typename T>
	void Activate(T* states, int target)
	{
		/*
		 * example
		 * states = 10001101
		 * target = 4
		 * 1 = 00000001
		 * 1 << target = 00010000
		 *
		 * 10001101 &
		 * 00010000 &
		 * -------- returns 1 where either are 1
		 * 10011101
		 */
		*states |= 1 << target;
	}

	// uniquely activate state (no others can be active)
	template<typename T>
	void UniquelyActivate(T* states, int target)
	{
		/*
		 * example
		 * target = 4
		 * 1 = 00000001
		 * 1 << target = 00010000
		 *
		 * 10000000 (ActiveState)
		 * 00010000 |
		 * -------- returns 1 where either are 1
		 * 10010000
		 * 00010000 &
		 * -------- returns 1 where both are 1
		 * 00010000
		 */
		Activate<T>(states, target); // activate state first
		*states &= 1 << target; // deactivate all others
	}

	template<typename T>
	void Deactivate(T* states, int target)
	{
		/*
		 * example
		 * states = 10011101
		 * target = 4
		 * 1 = 00000001
		 * 1 << target = 00010000
		 * ~(1 << target) = 11101111 flips bits
		 *
		 * 10011101
		 * 11101111 &
		 * --------
		 * 10001101 target bit is now deactivated
		 */
		*states &= ~(1 << target);
	}

	// toggle state
	template<typename T>
	void Toggle(T* states, int target)
	{
		/*
		 * example
		 * states = 10001101
		 * target = 4
		 * 1 = 00000001
		 * 1 << target = 00010000
		 *
		 * 10001101
		 * 00010000 ^
		 * -------- exclusive OR
		 * 10011101
		 */
		*states ^= 1 << target;
	}

}