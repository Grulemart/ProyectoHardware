#ifndef __PLANIFICADOR_H__
#define __PLANIFICADOR_H__

#define WATCHDOG_TIME 0x4000000
#define REPEAT_ALARM 0x80000000
#define USUARIO_AUSENTE 12000 // Tiempo a transcurrir entre inactividad para activar power-down mode (12 s)

void planificador(void);

#endif
