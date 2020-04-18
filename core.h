/* "CPU" emulation of a standard CHIP-8 VM.
 *
 * Derived from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

#include "graphics.h"

#define NUM_REGISTERS 16
#define STACK_LENGTH 16 
#define RAM_LENGTH_IN_BYTES 4096
#define PROGRAM_START_OFFSET 512

typedef uint8_t reg;
typedef uint8_t ubyte;
typedef uint16_t instruction;
typedef uint16_t ireg;

typedef struct core {
  ubyte ram[RAM_LENGTH_IN_BYTES];
  reg V[NUM_REGISTERS];
  ireg I; // generally holds memory addresses
  reg DT; // Delay timer
  reg ST; // Sound timer
  ireg PC; // currently holding address (program counter)
  ireg stack[STACK_LENGTH];
  reg SP; // stack pointer - points to the topmost level of the stack
} core;

ubyte* program_start_in_core(core* c) {
  if (c == NULL) 
    return NULL;

  return &(c->ram[PROGRAM_START_OFFSET]);
}

// More Significant Nibble and Less Significant Nibble
#define more_significant_nibble_of(u) (((u) >> 4) & 0xF)
#define less_significant_nibble_of(u) ((u) & 0xF)

void initialize_core(core* cpu) {
  cpu->PC = PROGRAM_START_OFFSET;
}

void tick(core* cpu, graphics* gpu) {
  instruction current_instruction = *(instruction *)(&(cpu->ram[cpu->PC]));
  ubyte lsb = (current_instruction >> 8) & 0xFF;
  ubyte msb = current_instruction & 0xFF;
  
  printf("%.2x%.2x\n", msb, lsb);
  if (msb == 0x00 && lsb == 0xe0) {
    // CLS - clear screen
    clear_graphics(gpu);
  } else if (msb == 0x00 && lsb == 0xee) {
    // RET - return from a subroutine
    cpu->SP -= 1;
    cpu->PC = cpu->stack[cpu->SP];
    // We store the PC from call which gets incremented to the
    // correct value at the end of tick.
  } else if (more_significant_nibble_of(msb) == 0) {
    // SYS - jump to a routine, deprecated
  } else if (more_significant_nibble_of(msb) == 1) {
    // JP - jump to a routine
    cpu->PC = (less_significant_nibble_of(msb) << 8) + (lsb & 0xFF);
    // Since we increment at the end of tick, we decrement here.
    cpu->PC -= sizeof(instruction);    
  } else if (more_significant_nibble_of(msb) == 2) {
    // CALL - call subroutine
    cpu->stack[cpu->SP] = cpu->PC;
    cpu->SP += 1;
    cpu->PC = (less_significant_nibble_of(msb) << 8) + (lsb & 0xFF);
    // Since we increment at the end of tick, we decrement here.
    cpu->PC -= sizeof(instruction);    
  } else if (more_significant_nibble_of(msb) == 3) {
    // SE - skip next instruction if equal
    if (cpu->V[less_significant_nibble_of(msb)] == lsb) {
      cpu->PC += sizeof(instruction);
    }
  } else if (more_significant_nibble_of(msb) == 4) {
    // SNE - skip next instruction if not equal
    if (cpu->V[less_significant_nibble_of(msb)] != lsb) {
      cpu->PC += sizeof(instruction);
    }
  } else if (more_significant_nibble_of(msb) == 5 && (less_significant_nibble_of(lsb) == 0)) {
    // SE - skip next instruction if registers are equal
    if (cpu->V[less_significant_nibble_of(msb)] == cpu->V[more_significant_nibble_of(lsb)])
      cpu->PC += sizeof(instruction);
  } else if (more_significant_nibble_of(msb) == 6) {
    // LD - load byte into register
    cpu->V[less_significant_nibble_of(msb)] = lsb;
  } else if (more_significant_nibble_of(msb) == 7) {
    // ADD - add byte into register
    cpu->V[less_significant_nibble_of(msb)] += lsb;
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 0)) {
    // LD - load register into other register
    cpu->V[less_significant_nibble_of(msb)] = cpu->V[more_significant_nibble_of(lsb)];
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 1)) {
    // OR - bitwise or two registers into another register
    cpu->V[less_significant_nibble_of(msb)] = cpu->V[less_significant_nibble_of(msb)] | cpu->V[more_significant_nibble_of(lsb)];
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 2)) {
    // AND - bitwise and two registers into another register
    cpu->V[less_significant_nibble_of(msb)] = cpu->V[less_significant_nibble_of(msb)] & cpu->V[more_significant_nibble_of(lsb)];
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 3)) {

  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 4)) {

  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 5)) {

  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 6)) {

  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 7)) {

  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 0xE)) {

  } else if (more_significant_nibble_of(msb) == 9 && (less_significant_nibble_of(lsb) == 0)) {
    // SNE - skip next instruction if registers are not equal
    if (cpu->V[less_significant_nibble_of(msb)] != cpu->V[more_significant_nibble_of(lsb)]) {
      cpu->PC += sizeof(instruction);
    }
  } else if (more_significant_nibble_of(msb) == 0xA) {

  } else if (more_significant_nibble_of(msb) == 0xB) {

  } else if (more_significant_nibble_of(msb) == 0xC) {

  } else if (more_significant_nibble_of(msb) == 0xD) {

  } else if (more_significant_nibble_of(msb) == 0xE && (lsb == 0x9E)) {

  } else if (more_significant_nibble_of(msb) == 0xE && (lsb == 0xA1)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x07)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x0A)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x15)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x18)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x1E)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x29)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x33)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x55)) {

  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x65)) {

  } else {
    printf("Invalid opcode: %.2x%.2x\n", msb, lsb);
  }
  cpu->PC += sizeof(instruction);
}
