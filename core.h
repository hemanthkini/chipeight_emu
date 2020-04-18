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
    // TODO
  } else if (msb == 0x00 && lsb == 0xee) {
    // RET - return from a subroutine
    cpu->SP -= 1;
    cpu->PC = cpu->stack[cpu->SP];
    // We store the PC from call which gets incremented to the
    // correct value at the end of tick.
  } else if (((msb >> 4) & 0xF) == 0) {
    // SYS - jump to a routine, deprecated
  } else if (((msb >> 4) & 0xF) == 1) {
    // JP - jump to a routine
    cpu->PC = ((msb & 0x0F) << 8) + (lsb & 0xFF);
    // Since we increment at the end of tick, we decrement here.
    cpu->PC -= sizeof(instruction);    
  } else if (((msb >> 4) & 0xF) == 2) {
    // CALL - call subroutine
    cpu->stack[cpu->SP] = cpu->PC;
    cpu->SP += 1;
    cpu->PC = ((msb & 0x0F) << 8) + (lsb & 0xFF);
    // Since we increment at the end of tick, we decrement here.
    cpu->PC -= sizeof(instruction);    
  } else if (((msb >> 4) & 0xF) == 3) {
    // SE - skip next instruction if equal
    if (cpu->V[msb & 0xF] == lsb) {
      cpu->PC += sizeof(instruction);
    }
  } else if (((msb >> 4) & 0xF) == 4) {
    // SNE - skip next instruction if not equal
    if (cpu->V[msb & 0xF] != lsb) {
      cpu->PC += sizeof(instruction);
    }
  } else if (((msb >> 4) & 0xF) == 5 && ((lsb & 0xF) == 0)) {
    // SE - skip next instruction if registers are equal
    if (cpu->V[msb & 0xF] == cpu->V[(lsb >> 4) & 0xF])
      cpu->PC += sizeof(instruction);
  } else if (((msb >> 4) & 0xF) == 6) {
    // LD - load byte into register
    cpu->V[msb & 0xF] = lsb;
  } else if (((msb >> 4) & 0xF) == 7) {
    // ADD - add byte into register
    cpu->V[msb & 0xF] += lsb;
  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 0)) {
    // LD - load register into other register
    cpu->V[msb & 0xF] = cpu->V[(lsb >> 4) & 0xF];
  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 1)) {
    // OR - bitwise or two registers into another register
    cpu->V[msb & 0xF] = cpu->V[msb & 0xF] | cpu->V[(lsb >> 4) & 0xF];
  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 2)) {
    // AND - bitwise and two registers into another register
    cpu->V[msb & 0xF] = cpu->V[msb & 0xF] & cpu->V[(lsb >> 4) & 0xF];
  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 3)) {

  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 4)) {

  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 5)) {

  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 6)) {

  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 7)) {

  } else if (((msb >> 4) & 0xF) == 8 && ((lsb & 0xF) == 0xE)) {

  } else if (((msb >> 4) & 0xF) == 9 && ((lsb & 0xF) == 0)) {
    // SNE - skip next instruction if registers are not equal
    if (cpu->V[msb & 0xF] != cpu->V[(lsb >> 4) & 0xF]) {
      cpu->PC += sizeof(instruction);
    }
  } else if (((msb >> 4) & 0xF) == 0xA) {

  } else if (((msb >> 4) & 0xF) == 0xB) {

  } else if (((msb >> 4) & 0xF) == 0xC) {

  } else if (((msb >> 4) & 0xF) == 0xD) {

  } else if (((msb >> 4) & 0xF) == 0xE && (lsb == 0x9E)) {

  } else if (((msb >> 4) & 0xF) == 0xE && (lsb == 0xA1)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x07)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x0A)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x15)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x18)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x1E)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x29)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x33)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x55)) {

  } else if (((msb >> 4) & 0xF) == 0xF && (lsb == 0x65)) {

  } else {
    printf("Invalid opcode: %.2x%.2x\n", msb, lsb);
  }
  cpu->PC += sizeof(instruction);
}
