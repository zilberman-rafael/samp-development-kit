/**--------------------------------------------------------------------------**\
					    =======================
					    RLS Includes - Bit Core
					    =======================
Description:
	Provides functions for bit manipulation and bit arrays greater than 32bits.
	The arrays are usually bigger than required due to cell boundaries but this
	shouldn't cause a major problem (bit tests on the 101st bit of a 100 bit
	array won't return 0 for out of bounds, but the 129th will).
	
	Note that y_commands has a few optimisations which bypass the code in here
	so any modifications to bit array layouts will need to be reflected there.
Legal:
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	Contributors:
		ZeeX, koolk, JoeBullet/Google63, g_aSlice/Slice, Y_LESS
	
	Very special thanks to:
		Y_LESS - The original programmer of this include.
		Kye/Kalcor - SA:MP.
		SA:MP Team past, present and future - SA:MP.
	
Version:
	0.1
Changelog:
	16/06/2014:
		First version.
Functions:
	Public:
		-
	Core:
		-
	Stock:
		-
	Static:
		-
	Inline:
		Bit_Cells - Gets the number of cells required for a bit array.
		Bit_Size - Gets the size of the bit array.
	API:
		-
Callbacks:
	-
Definitions:
	CELLSHIFT - Number of bits that can hold "cellbits"
Enums:
	-
Macros:
	-
Tags:
	Bit - Bit array type.
Variables:
	Global:
		-
	Static:
		-
Commands:
	-
Compile options:
	-
</remarks>
\**--------------------------------------------------------------------------**/

#if defined _RLS_@INCLUDE_@@bitarray
	#endinput
#endif
#define _RLS_@INCLUDE_@@bitarray

// 32 bits comes first because its most likley that the current machine is 32 bit
#if cellbits == 32
	#define CELLSHIFT (5)
#else
	#if cellbits == 64
		#define CELLSHIFT (6)
	#else
		#if cellbits == 16
			#define CELLSHIFT (4)
		#else
			#if cellbits == 8
				#define CELLSHIFT (3)
			#else
				#if cellbits == 4
					#define CELLSHIFT (2)
				#else
					#if cellbits == 2
						#define CELLSHIFT (1)
					#else
						#error Unkown cell size
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

/**--------------------------------------------------------------------------**\
<summary>Bit_Cells</summary>
<param name="size">Number of bits required.</param>
<returns>
	Number of cells required for the bit array.
</returns>
<remarks>
	-
</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_Cells(%1) (((%1)+cellbits-1)/cellbits)

/**--------------------------------------------------------------------------**\
<summary>Bit_Slot</summary>
<param name="value">Value to get the slot for.</param>
<returns>
	The true array slot for this value.
</returns>
<remarks>
	-
</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_Slot(%1) ((_:%1)>>>CELLSHIFT)

/**--------------------------------------------------------------------------**\
<summary>Bit_Size</summary>
<param name="Bit:array[]">Array of bits.</param>
<returns>
	Size of the bit array
</returns>
<remarks>
	-
</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_Size(%1) (sizeof(%1)*cellbits)

/**--------------------------------------------------------------------------**\
<summary>Bit_Get</summary>
<param name="Bit:array[]">Array of bits.</param>
<param name="slot">Bit slot.</param>
<returns>
	State of the provided slot
</returns>
<remarks>
	Unsafe, make sure you're within range.
</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_Get(%1,%2) (%1[Bit_Slot(%2)]&Bit:(1<<((%2)&cellbits-1)))

/**--------------------------------------------------------------------------**\
<summary>Bit_Set</summary>
<param name="Bit:array[]">Array of bits.</param>
<param name="slot">Bit slot.</param>
<param name="bool:state">State to set the slot to.</param>
<returns>
	-
</returns>
<remarks>
	-

native Bit_Set(BitArray:array<>, slot, bool:state);

</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_Set(%0,%1,%2) ((%2)?(Bit_On(%0,(%1))):(Bit_Of(%0,(%1))))

/**--------------------------------------------------------------------------**\
<summary>Bit_On</summary>
<param name="Bit:array[]">Array of bits.</param>
<param name="slot">Bit slot.</param>
<returns>
	-
</returns>
<remarks>
	Sets the slot to 1.
</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_On(%1,%2) %1[Bit_Slot(%2)]|=Bit:(1<<((%2)&cellbits-1))

/**--------------------------------------------------------------------------**\
<summary>Bit_Off</summary>
<param name="Bit:array[]">Array of bits.</param>
<param name="slot">Bit slot.</param>
<returns>
	-
</returns>
<remarks>
	Sets the slot to 0.
</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_Off(%1,%2) %1[Bit_Slot(%2)]&=Bit:~(1<<((%2)&cellbits-1))

/**--------------------------------------------------------------------------**\
<summary>Bit_SetAll</summary>
<param name="Bit:array[]">Array to set all values of.</param>
<param name="bool:state">Wether to set them all 0 or 1.</param>
<returns>
	-
</returns>
<remarks>
	-

native Bit_SetAll(BitArray:array<>, bool:state);

</remarks>
\**--------------------------------------------------------------------------**/

#define Bit_SetAll(%1,%2) (memset(_:%1, ((%2) ? (0xFFFFFFFF) : (0x0)), size))/**--------------------------------------------------------------------------**\
<summary>Bit_Count</summary>
<param name="Bit:array[]">Array to count.</param>
<param name="size">Size of array.</param>
<returns>
	Number of 1s or 0s in the array.
</returns>
<remarks>
	Code from:
	http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel

native Bit_Count(Bit:array[], bool:state = true);

</remarks>
\**--------------------------------------------------------------------------**/

stock Bit_Count(Bit:array[], bool:state = true)
{
	new
		count,
		size = Bit_Size(array);
	for (new rl@bitarray@@itter = 0; rl@bitarray@@itter < size; i++)
	{
		count += array[Bit_Slot(i)];
	}
	return abs((!state)*size - count);
}