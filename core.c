#include "core.h"
#include "graphics.h"

ubyte* program_start_in_core(core* c) {
  if (c == NULL)
    return NULL;

  return &(c->ram[PROGRAM_START_OFFSET]);
}

void initialize_core(core* cpu) {
  cpu->PC = PROGRAM_START_OFFSET;

  sranddev();
  load_sprites_into_pointer(&(cpu->ram[HEX_SPRITE_START_OFFSET]));
}

void tick(core* cpu, graphics* gpu) {
  instruction current_instruction = *(instruction *)(&(cpu->ram[cpu->PC]));
  ubyte lsb = (current_instruction >> 8) & 0xFF;
  ubyte msb = current_instruction & 0xFF;

  //printf("%.2x%.2x\n", msb, lsb);
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
    // XOR - xor two registers
    cpu->V[less_significant_nibble_of(msb)] = cpu->V[less_significant_nibble_of(msb)] ^ cpu->V[more_significant_nibble_of(lsb)];
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 4)) {
    // ADD - add two registers, set carry bit in VF
    cpu->V[0xF] = 0;
    uint16_t result = (uint16_t)cpu->V[less_significant_nibble_of(msb)] + (uint16_t)cpu->V[more_significant_nibble_of(lsb)];
    if (result > (uint16_t)0xFF) {
      cpu->V[0xF] = 1;
    }
    cpu->V[less_significant_nibble_of(msb)] = (reg)(result & 0x00FF);
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 5)) {
    // SUB - sub two registers, set NOT borrow bit in VF
    cpu->V[0xF] = 0;
    if (cpu->V[less_significant_nibble_of(msb)] > cpu->V[more_significant_nibble_of(lsb)]) {
      cpu->V[0xF] = 1;
    }

    uint16_t result = (uint16_t)cpu->V[less_significant_nibble_of(msb)] - (uint16_t)cpu->V[more_significant_nibble_of(lsb)];
    cpu->V[less_significant_nibble_of(msb)] = (reg)(result & 0x00FF);
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 6)) {
    // SHR - shift right by 1, set VF if odd
    cpu->V[0xF] = 0;
    if (cpu->V[less_significant_nibble_of(msb)] & 0x1) {
      cpu->V[0xF] = 1;
    }

    cpu->V[less_significant_nibble_of(msb)] = (cpu->V[less_significant_nibble_of(msb)] >> 1);
  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 7)) {
    // SUBN - sub two registers, set NOT borrow bit in VF
    cpu->V[0xF] = 0;
    if (cpu->V[more_significant_nibble_of(lsb)] > cpu->V[less_significant_nibble_of(msb)]) {
      cpu->V[0xF] = 1;
    }

    uint16_t result = (uint16_t)cpu->V[more_significant_nibble_of(lsb)] - (uint16_t)cpu->V[less_significant_nibble_of(msb)];
    cpu->V[less_significant_nibble_of(msb)] = (reg)(result & 0x00FF);

  } else if (more_significant_nibble_of(msb) == 8 && (less_significant_nibble_of(lsb) == 0xE)) {
    // SHR - shift left by 1, set VF if most significant bit is 1
    cpu->V[0xF] = 0;
    if (cpu->V[less_significant_nibble_of(msb)] & 0x80) {
      cpu->V[0xF] = 1;
    }
    cpu->V[less_significant_nibble_of(msb)] = (cpu->V[less_significant_nibble_of(msb)] << 1);
  } else if (more_significant_nibble_of(msb) == 9 && (less_significant_nibble_of(lsb) == 0)) {
    // SNE - skip next instruction if registers are not equal
    if (cpu->V[less_significant_nibble_of(msb)] != cpu->V[more_significant_nibble_of(lsb)]) {
      cpu->PC += sizeof(instruction);
    }
  } else if (more_significant_nibble_of(msb) == 0xA) {
    // LD I - set I = nnn
    cpu->I = (less_significant_nibble_of(msb) << 8) + (lsb & 0xFF);
  } else if (more_significant_nibble_of(msb) == 0xB) {
    // JP - jump to V0 + nnn
    cpu->PC = cpu->V[0] + (less_significant_nibble_of(msb) << 8) + (lsb & 0xFF);
    cpu->PC -= sizeof(instruction);
  } else if (more_significant_nibble_of(msb) == 0xC) {
    // RND - set Vx = random byte & lsb
    // Not the most equally distributed rand() but whatever.
    cpu->V[less_significant_nibble_of(msb)] = (reg)(rand() & lsb);
  } else if (more_significant_nibble_of(msb) == 0xD) {
    cpu->V[0xF] = place_sprite(cpu, gpu, less_significant_nibble_of(lsb), cpu->V[less_significant_nibble_of(msb)], cpu->V[more_significant_nibble_of(lsb)]);
  } else if (more_significant_nibble_of(msb) == 0xE && (lsb == 0x9E)) {
    printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xE && (lsb == 0xA1)) {
    printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x07)) {
    printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x0A)) {
    // TODO if key-press, store value of key in Vx
    // else, redo instruction: 
    cpu->PC -= sizeof(instruction);
    //printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x15)) {
    printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x18)) {
    printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x1E)) {
    printf("Unimplemented opcode: %.2x%.2x\n", msb, lsb);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x29)) {
    cpu->I = HEX_SPRITE_START_OFFSET + (cpu->V[less_significant_nibble_of(msb)] * HEX_SPRITE_BYTES_ON_DISK);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x33)) {
    uint16_t value = cpu->V[less_significant_nibble_of(msb)];
    cpu->ram[cpu->I] = (value / 100) % 10;
    cpu->ram[cpu->I + 1] = (value / 10) % 10;
    cpu->ram[cpu->I + 2] = value % 10;
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x55)) {
    // LD - Load [I] from registers V0 - Vx
    memcpy(&(cpu->ram[cpu->I]), &(cpu->V[0]), less_significant_nibble_of(msb) + 1);
  } else if (more_significant_nibble_of(msb) == 0xF && (lsb == 0x65)) {
    // LD - Load registers V0 - Vx from [I]
    memcpy(&(cpu->V[0]), &(cpu->ram[cpu->I]), less_significant_nibble_of(msb) + 1);
  } else {
    printf("Invalid opcode: %.2x%.2x\n", msb, lsb);
  }
  cpu->PC += sizeof(instruction);
}
