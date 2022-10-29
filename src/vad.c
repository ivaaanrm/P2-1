#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "pav_analysis.h"
#include "vad.h"

const float FRAME_TIME = 10.0F; /* in ms. */
const int MAX_TRAMAS = 10;

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  feat.zcr = compute_zcr(x,N,16000);
  feat.p = compute_power(x,N);
  feat.am = compute_am(x,N);
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate, float alpha1, float alpha2) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  vad_data->alpha1 = alpha1;
  vad_data->alpha2 = alpha2;
  vad_data->counter = 0;
  vad_data->MAX_MB = 5;
  vad_data->MIN_VOICE = 30;
  vad_data->MIN_SILENCE = 10;
  vad_data->N_TRAMAS = MAX_TRAMAS;

  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */

  switch (vad_data->state) {
  case ST_INIT:
    vad_data->k1  = f.p + vad_data->alpha1;
    vad_data->k2  = f.p + vad_data->alpha2;
    vad_data->state = ST_SILENCE;
    break;

  case ST_SILENCE:
    if (f.p > vad_data->k2)
      vad_data->state = ST_MBV;
    else if (f.p > vad_data->k1){
      vad_data->N_TRAMAS --;
      vad_data->state = ST_MBS;}
    break;

  case ST_VOICE:
    if (f.p < vad_data->k1)
      vad_data->state = ST_MBS;
    else if (f.p < vad_data->k2)
      vad_data->state = ST_MBV;
    break;

  case ST_MBV:
    if (f.p > vad_data->k2)
      vad_data->state = ST_VOICE;
    else if ((f.p < vad_data->k1) || (vad_data->N_TRAMAS == 0)){
      vad_data->state = ST_SILENCE;
      vad_data->N_TRAMAS = MAX_TRAMAS;}
    else
      vad_data->N_TRAMAS--;
    break;
  
  case ST_MBS:
    if ((f.p > vad_data->k2) || (vad_data->N_TRAMAS == 0)){
      vad_data->state = ST_VOICE;
      vad_data->N_TRAMAS = MAX_TRAMAS;}
    else if (f.p < vad_data->k1)
      vad_data->state = ST_SILENCE;
    else
      vad_data->N_TRAMAS--;
    break;

  case ST_UNDEF:
    break;
  }

  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_MBV)
    return ST_SILENCE;
  else if (vad_data->state == ST_VOICE ||
      vad_data->state == ST_MBS)
    return ST_VOICE;
  
  else
    return ST_UNDEF;
}

void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}