#ifndef MACHINE_CONSTANTS_HPP
#define MACHINE_CONSTANTS_HPP

#include <array>

#include "types.hpp"

/* Constants for the machine */
namespace MachineConstants {
/* Speed of the CPU. This speed is not the same as the COSMAC VIP or other
 * computer because in this interpreter all instructions take 1 cycle */
constexpr uint32_t cpuSpeed = 500; /* Hz */
/* Number of rows of each font character */
constexpr auto fontCharHeight = 5;
/* Display parameters */
constexpr auto displayWidth = 64;
constexpr auto displayHeight = 32;
constexpr auto displaySize = displayWidth * displayHeight;
/* Important memory addresses and sizes (in bytes) */
/* Fontset */
constexpr auto fontsetMemAdr = 0x50;
constexpr auto fontsetMemSiz = 80;
/* Start of the programs */
constexpr auto prgStartMemAdr = 0x200;
constexpr auto prgLimitMemAdr = 0xFFF;
/* Machine status register */
constexpr auto statusRegMemAdr = 0x1000;
constexpr auto statusRegMemSiz = 1;
/* IO registers (1001 & 1002) (used for the keyboard) */
constexpr auto ioRegsMemAdr = 0x1001;
constexpr auto ioRegsMemSiz = 2;
/* Display memory */
constexpr auto displayMemAdr = 0x1003;
constexpr auto displayMemSiz = displaySize / 8;
/* The auxiliary memory is an extension of the main memory that i use for hold
 * the io registers and the display memory. It would be an extra memory chip
 * connected to the bus.
 * The addresses between 0x0000 and 0x0FFF points to the main memory
 * The addresses between 0x1000 and 0x1200 points to the auxiliary memory */
static constexpr auto auxMemStartAdr = 0x1000;
static constexpr auto auxMemSiz =
    statusRegMemSiz + ioRegsMemSiz + displayMemSiz;
/* Memory limit. This address is not addressable */
static constexpr auto memAdrLimit = auxMemStartAdr + auxMemSiz;
static constexpr auto displayMemAdrLimit = memAdrLimit;

/* The machine status register stores the current status of the machine
 *   0x1000
 * xxxxxxds
 * d is set when the display needs to be updated
 * s is set when the speaker must sound */
constexpr byte_t setSoundMask = 0x01;
constexpr byte_t resetSoundMask = 0xFE;
constexpr byte_t setDrawMask = 0x02;
constexpr byte_t resetDrawMask = 0xFD;

/* Keys */
/* The keyboard of this machine has this layout:
 * 1 2 3 C
 * 4 5 6 D
 * 7 8 9 E
 * A 0 B F */
constexpr auto numOfKeys = 16;
/* The IO registers stores the keys as follows:
 *     0x1001   0x1002
 *   01234567 89ABCDEF*/
constexpr word_t key0Mask = 0x80;
constexpr word_t key1Mask = 0x40;
constexpr word_t key2Mask = 0x20;
constexpr word_t key3Mask = 0x10;
constexpr word_t key4Mask = 0x08;
constexpr word_t key5Mask = 0x04;
constexpr word_t key6Mask = 0x02;
constexpr word_t key7Mask = 0x01;
constexpr word_t key8Mask = 0x80;
constexpr word_t key9Mask = 0x40;
constexpr word_t keyAMask = 0x20;
constexpr word_t keyBMask = 0x10;
constexpr word_t keyCMask = 0x08;
constexpr word_t keyDMask = 0x04;
constexpr word_t keyEMask = 0x02;
constexpr word_t keyFMask = 0x01;
constexpr std::array<word_t, numOfKeys> keyMasks{
    key0Mask, key1Mask, key2Mask, key3Mask, key4Mask, key5Mask,
    key6Mask, key7Mask, key8Mask, key9Mask, keyAMask, keyBMask,
    keyCMask, keyDMask, keyEMask, keyFMask};

}  // namespace MachineConstants
#endif /* MACHINE_CONSTANTS_HPP */
