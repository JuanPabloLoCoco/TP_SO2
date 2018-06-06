#include <naiveConsole.h>
#include <lib.h>
#include <videoDriver.h>
#include <KeyboardDriver.h>
#include <KeyboardMap.h>
#include <time.h>
#include <process.h>
#include <defs.h>

static char buffer[BUFFERSIZE];
static unsigned int ret_index = 0;
static unsigned int store_index = 0;
static int buff_size = 0;
static int shiftPressed = FALSE;
static int capsLockPressed = FALSE;
static int capsLockPressedCount = 0;
static int capsLockActivated = FALSE;

uint16_t index = 0;
static int count = 0;
uint16_t read = 0;


void keyboard_handler()
{
	if (buff_size == BUFFERSIZE)
	{
		return;
	}

	char k = getKey();
	if (k == ESCAPE)
	{
    set_foreground_force_process(get_process_by_pid(ppid_process(get_foreground_process())));
    return;
  }

  if (k > 0 && k < TOPCHARCODE)
	{
		/* Se apretó una tecla */
    if(!processKeyScanCode(k))
		{
			//caso de un caracter a imprimir en pantalla
			count++;
	    
			buff_size++;
			// guarda el scancode de la tecla
			buffer[store_index++] = k;

      unblock_read_process(get_foreground_process());
		}
  }
	if(k > 0 && k == BACKSPACECODE) {
	    buffer[index] = '\b';
	    incrementIndex();
	    if(count > 0){
	      count--;
	      erase_char();
	    }

	  }
	else if (k < 0)
	{
			/*Se solto una tecla */
      processKeyScanCode(k + TOPCHARCODE);
  }
  if (store_index == BUFFERSIZE)
	{
		store_index = 0;
	}
}

/* Obtiene la tecla correspondiente segun los flags activados */
int getChar()
{
  if (buff_size == 0)
	{
		return -1;
	}
  char k = buffer[ret_index++];
  buff_size--;
  if (ret_index == BUFFERSIZE)
  {
		ret_index = 0;
	}
  return chooseKeyboard(k);
}

/* Retorna 1 si el scancode recibido es de Shift */
int isShiftKey(char k)
{
 return ((k == LSHIFTCODE) || (k == RSHIFTCODE));
}

/* Retorna TRUE si se toco algun caracter especial como SHIFT, CAPSLOCK y modifica los flags correspondientes*/
int processKeyScanCode(char k)
{
  switch(k)
	{
    case RSHIFTCODE:
          shiftPressed = !shiftPressed;
          return TRUE;
          break;
    case LSHIFTCODE:
          shiftPressed = !shiftPressed;
          return TRUE;
          break;
    case CAPSLOCK:
          capsLockPressed = !capsLockPressed;
          capsLockPressedCount += 1;
          if(capsLockPressedCount % 2 == 0)
					{
						//fue apretado y soltado
            capsLockActivated = !capsLockActivated;
            capsLockPressedCount = 0;
          }
          return TRUE;
          break;
    default:
          return FALSE;
          break;
  }
  return FALSE;
}

/* Retorna el caracter pedido eligiendo el teclado segun los flags que se encuentran activados.
** en caso de no encontrarse el caracter retorna -1 */
int chooseKeyboard(int k)
{
  if (kbdus[k] == 0)
	{// no es imprimible
    return -1;
	}
	if(capsLockActivated && !shiftPressed)
  {
		return capskbdus[k];
	}
  if(!capsLockActivated && shiftPressed)
  {
		return shiftkbdus[k];
	}
  if(capsLockActivated && shiftPressed)
  {
		return isAlpha(k) ? kbdus[k] : shiftkbdus[k];
	}
  return kbdus[k];
}

/* Retorna 1 si el scancode recibido equivale a una letra */
int isAlpha(char k)
{
  if(( k >= QPOSITION && k <= PPOSITION) || (k >= APOSITION && k <= LPOSITION) || (k >= ZPOSITION && k <= MPOSITION))
	{
		return TRUE;
	}
  return FALSE;
}

void incrementIndex(){
  if((index + 1 %256) != read) {
    index ++;
    index = index%256;
  }
}
