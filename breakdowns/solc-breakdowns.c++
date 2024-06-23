0x6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220f4cc721dc409b528475d0864e930edfca7075364564549d6a1e547f09bdb547a64736f6c63430008140033

// 1.Contract Creation Code
// 2.Runtime
// 3.Metadata



// 1.Contract Creation Code

// Free memory pointer
PUSH1 0x80          // [0x80]
PUSH1 0x40          // [0x40, 0x80]
MSTORE              // stack : [], memory = {0x80} // 0x40 is the reservered slot for the solidity free memory pointer
                                                   // whatever the value in 0x40 is, it is saying that the index is free for to use in memory
                                                   // 0x00 and 0x20 are reserved for hashing stuff, precomplies in memory
                                                   // 0x60 in memory is also reserved
                                                   // if we update memory, 0x40 which contains the memory pointer is also updated



// If someone sent value with this call revert!
// Otherwise jump
CALLVALUE          //  [msg.value]
DUP1               //  [msg.value, msg.value]
ISZERO             //  [msg.value == 0/1, msg.value]
PUSH1 0x0e         //  [0x0e, msg.value == 0/1, msg.value]
JUMPI              //  [msg.value]

PUSH0              //  [0x00, msg.value]
DUP1               //  [0x00, 0x00, msg.value]
REVERT             //  [msg.value]

// Jump Dest if msg.value == 0
// sticks the runtime code on chain
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0xa5          // [0xa5]
DUP1                // [0xa5, 0xa5]
PUSH2 0x001b        // [0x001b, 0xa5, 0xa5]
PUSH0               // [0x00, 0x001b, 0xa5, 0xa5] // The 0x00 is the starting loc,  The 0x001b is pointing to the code below Invalid // 0xa5 is the byte size to copy
CODECOPY            // stack : [0xa5],  Memory : {runtime code}
PUSH0               // [0x00, 0xa5] , we are returning the whole runtime code size
RETURN              // []
INVALID             // []


// Runtime Code
PUSH1 0x80          // [0x80]
PUSH1 0x40          // [0x40]
MSTORE              // [], memory : {0x40 : 0x80}

// if no functions are payable, it knows to revert if someone sends value
CALLVALUE           // [msg.value]
DUP1                // [msg.value, msg.value]
ISZERO              // [msg.value == 0/1, msg.value]
PUSH1 0x0e          // [0x0e, msg.value == 0/1, msg.value] // if msg.value is 0 go to 0x0e on JUMPI
JUMPI               // [msg.value]
PUSH0               // [0x00, msg.value]
DUP1                // [0x00, 0x00, msg.value]
REVERT              // [msg.value]

// if msg.value == 0, start here!
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0x04          // [0x04]
CALLDATASIZE        // [calldata_size, 0x04]
LT                  // [calldata_size < 0x04]
PUSH1 0x30          // [0x30, calldata_size < 0x04]
JUMPI               // [] // we will jump if calldata is small which means calldata is not 4 bytes long and no function is sent in the calldata


// Function dispatching
PUSH0               // [0x00]
CALLDATALOAD        // [calldata]
PUSH1 0xe0          // [0xe0, calldata]
SHR                 // [function_selector]
DUP1                // [function_selector, function_selector]
PUSH4 0xcdfead2e    // [0xcdfead2e, function_selector, function_selector]
EQ                  // [1/0, function_selector]
PUSH1 0x34          // [0x34, 1/0, function_selector]
JUMPI               // [function_selector] and Jump to the program counter if 1


DUP1                // [function_selector, function_selector]
PUSH4 0xe026c017    // [0xe026c017, function_selector, function_selector]
EQ                  // [1/0, function_selector]
PUSH1 0x45          // [0x45, 1/0, function_selector]
JUMPI               // [function_selector] and Jump to the program counter if 1

// calldata_jump
JUMPDEST        // []
PUSH0           // [0x00]
DUP1            // [0x00, 0x00]
REVERT

// updateHorseNumber jump dest 1
JUMPDEST            // [function_selector]
PUSH1 0x43          // [0x43, function_selector]
PUSH1 0x3f          // [0x3f, 0x43, function_selector]
CALLDATASIZE        // [calldata_size, 0x3f, 0x43, function_selector]
PUSH1 0x04          // [0x04, calldata_size, 0x3f, 0x43, function_selector]
PUSH1 0x59          // [0x59, 0x04, calldata_size, 0x3f, 0x43, function_selector], 
JUMP                // Jumps to the 0x59 counter

// Jump dest 4
JUMPDEST            // [calldata_data, 0x43, function_selector]
PUSH0               // [0x00, calldata_data, 0x43, function_selector]
SSTORE              // [0x43, function_selector]
JUMP                // [function_selector] jump to 0x43

// Jump dest 5
JUMPDEST            // [function_selector]
STOP                // [function_selector]

// getHorseNumber jump
JUMPDEST            // [function_selector]            
PUSH0               // [0x00, function_selector]
SLOAD               // [numHorses, function_selector]
PUSH1 0x40          // [0x40, numHorses, function_selector]
MLOAD               // [0x80, numHorses, function_selector] // free memory pointer is loaded
SWAP1               // [numHorses, 0x80, function_selector]
DUP2                // [numHorses, 0x80, 0x80, function_selector]
MSTORE              // stack : [0x80, function_selector], memory : {0x80: numHorses}
PUSH1 0x20          // stack : [0x20, 0x80, function_selector], memory : {0x80: numHorses}
ADD                 // stack : [0xa0, function_selector], memory : {0x80: numHorses}
PUSH1 0x40          // stack : [0x40, 0xa0, function_selector], memory : {0x80: numHorses}
MLOAD               // stack : [0x80, 0xa0, function_selector], memory : {0x80: numHorses}
DUP1                // stack : [0x80, 0x80, 0xa0, function_selector], memory : {0x80: numHorses}
SWAP2               // stack : [0xa0, 0x80, 0x80, function_selector], memory : {0x80: numHorses}
SUB                 // stack : [0x20, 0x80, function_selector], memory : {0x80: numHorses}
SWAP1               // stack : [0x80, 0x20, function_selector], memory : {0x80: numHorses}

// return a value of size 32 bytes which is at 0x80 in memory
RETURN              // stack : [function_selector] and returns memory

// updateHorseNumber jump dest 2
// check to see if there is a value to update the horse number to
JUMPDEST            // [0x04, calldata_size, 0x3f, 0x43, function_selector]
PUSH0               // [0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
PUSH1 0x20          // [0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
DUP3                // [0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
DUP5                // [calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
SUB                 // [calldata_size - 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
SLT                 // [((calldata_size - 0x04) < 0x20), 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector] 

// the SLT checks for size less than, it is checking if there is more calldata than the function selector?

ISZERO              // [more_calldata_than_selector?, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
PUSH1 0x68          // [0x68, more_calldata_than_selector, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
JUMPI               // [0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector], jump to 0x68 if there is more calldata than selector
// revert if there is not enough calldata
PUSH0               // [0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
DUP1                // [0x00, 0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
REVERT              // reverts

// jump dest 3
JUMPDEST            // [0x00, 0x04, calldata_size, 0x3f, 0x43, function_selector]
POP                 // [0x04, calldata_size, 0x3f, 0x43, function_selector]
// ignore the function selector and just grab the data
CALLDATALOAD        // [calldata_data, calldata_size, 0x3f, 0x43, function_selector]
SWAP2               // [0x3f, calldata_size, calldata_data, 0x43, function_selector]
SWAP1               // [calldata_size, 0x3f, calldata_data, 0x43, function_selector]
POP                 // [0x3f, calldata_data, 0x43, function_selector]
JUMP                // [calldata_data, 0x43, function_selector] Jump to 0x3f


// 3. MetaData
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
DELEGATECALL
INVALID
PUSH19 0x1dc409b528475d0864e930edfca70753645645
BLOBHASH
INVALID
LOG1
INVALID
SELFBALANCE
CREATE
SWAP12
INVALID
SLOAD
PUSH27 0x64736f6c63430008140033