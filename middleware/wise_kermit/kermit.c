/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if MIDDLEWARE_WISE_KERMIT
#define KERMIT_C
/*
  Embedded Kermit protocol module
  Version: 1.7
  Most Recent Update: Mon Jun  6 15:36:26 2011

  No stdio or other runtime library calls, no system calls, no system
  includes, no static data, and no global variables in this module.

  Warning: you can't use debug() in any routine whose argument list
  does not include "struct k_data * k".  Thus most routines in this
  module include this arg, even if they don't use it.

  Author: Frank da Cruz.
  As of version 1.6 of 30 March 2011, E-Kermit is Open Source software under
  the Revised 3-Clause BSD license which follows.  E-Kermit 1.6 is identical
  to version 1.51 except for the new license.

  Author: Frank da Cruz.

  Copyright (C) 1995, 2011,
  Trustees of Columbia University in the City of New York.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of Columbia University nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
// #include "cdefs.h"			/* C language defs for all modules */
#include <stdint.h>

// #include "debug.h"			/* Debugging */
#include "kermit.h" /* Kermit protocol definitions */

#define STATIC static
#define ENABLE_DEBUG 0

/*
  Never use NULL as a pointer.  Always use 0 cast to the appropriate type,
  for example: (UCHAR *)0.  Reason: compiler might define NULL to be an
  integer 0, and when passed to a function that wants a pointer, might wind
  up with junk in the left half (if pointers wider than ints).
*/
#ifdef NULL
#undef NULL
#endif /* NULL */

//#define zgetc() ((--(k->zincnt))>=0)?((int)(*(k->zinptr)++)&0xff):(*(k->readf))(k)

/* See cdefs.h for meaning of STATIC, ULONG, and UCHAR */

#if 0
STATIC ULONG stringnum(UCHAR *, struct k_data *);
STATIC UCHAR * numstring(ULONG, UCHAR *, int, struct k_data *);
#endif

int STATIC spkt(char, short, int, UCHAR *, struct k_data *);
int STATIC ack(struct k_data *, short, UCHAR *text);
int STATIC nak(struct k_data *, short);
int STATIC chk1(UCHAR *, struct k_data *);
STATIC USHORT chk2(UCHAR *, struct k_data *);
#ifdef F_CRC
STATIC USHORT chk3(UCHAR *, struct k_data *);
#endif /* F_CRC */
void STATIC spar(struct k_data *, UCHAR *, int);
int STATIC rpar(struct k_data *, char);
int STATIC decode(struct k_data *, struct k_response *, short, UCHAR *);
#ifdef F_AT
int STATIC gattr(struct k_data *, UCHAR *, struct k_response *);
int STATIC sattr(struct k_data *, struct k_response *);
#endif /* F_AT */
#ifndef RECVONLY
int STATIC sdata(struct k_data *, struct k_response *);
#endif /* RECVONLY */
void STATIC epkt(char *, struct k_data *);
// int STATIC getpkt(struct k_data *, struct k_response *);
// int STATIC encstr(UCHAR *, struct k_data *, struct k_response *);
// void STATIC decstr(UCHAR *, struct k_data *, struct k_response *);
void STATIC encode(int, int, struct k_data *);
// int STATIC nxtpkt(struct k_data *);
// int STATIC resend(struct k_data *);
#ifdef DEBUG
int xerror(void);
#endif /* DEBUG */

/* The kermit() function */
int kermit(short f,              /* Function code */
           struct k_data *k,     /* The control struct */
           int len,              /* Length of packet in slot */
           char *msg,            /* Message for error packet */
           struct k_response *r) /* Response struct */
{
    int i, rc;   /* Workers */
    int datalen; /* Length of packet data field */
    UCHAR *p;    /* Pointer to packet data field */
    UCHAR *q;    /* Pointer to data to be checked */
    // UCHAR *s;				/* Worker string pointer */
    UCHAR t;         /* Worker chars */
    UCHAR pbc[4];    /* Copy of packet block check */
    short seq, prev; /* Copies of sequence numbers */
    // short chklen;                       /* Length of packet block check */
#ifdef F_CRC
    unsigned int crc; /* 16-bit CRC */
#endif                /* F_CRC */
    int ok;

#if ENABLE_DEBUG
    debug(DB_MSG, "----------", 0, 0); /* Marks each entry */
    debug(DB_LOG, "f", 0, f);
    debug(DB_LOG, "state", 0, k->state);
    debug(DB_LOG, "zincnt", 0, (k->zincnt));
#endif

    if (f == K_INIT) { /* Initialize packet buffers etc */
        // k->version = (UCHAR *)VERSION;	/* Version of this module */
        r->filename[0] = '\0'; /* No filename yet. */
        // r->filedate[0] = '\0';		/* No filedate yet. */
        // r->filesize = 0L;               /* No filesize yet. */
        r->sofar = 0L; /* No bytes transferred yet */

        // for (i = 0; i < P_WSLOTS; i++) { /* Packet info for each window slot
        // */
        freerslot(k);
        // freesslot(k,i);
        // }
#ifdef F_TSW
        for (i = 0; i < 64; i++) { /* Packet finder array */
            k->r_pw[i] = -1;       /* initialized to "no packets yet" */
            k->s_pw[i] = -1;       /* initialized to "no packets yet" */
        }
#endif /* F_TSW */

        /* Initialize the k_data structure */

        // for(i = 0; i < 6; i++)
        //     k->s_remain[i] = '\0';

        k->state  = R_WAIT; /* Beginning protocol state */
        r->status = R_WAIT;
        // k->what     = W_RECV;		/* Default action */
        k->s_first = 1; /* Beginning of file */
        // k->r_soh    = k->s_soh = SOH;	/* Packet start */
        // k->r_eom    = k->s_eom = CR;	/* Packet end */
        k->s_eom = CR;             /* Packet end */
        k->s_seq = k->r_seq = 0;   /* Packet sequence number */
        k->s_type = k->r_type = 0; /* Packet type */
        // k->r_timo   = P_R_TIMO;		/* Timeout interval for me to use */
        k->s_timo = P_S_TIMO; /* Timeout for other Kermit to use */
        // k->r_maxlen = P_PKTLEN;         /* Maximum packet length */
        k->s_maxlen = P_PKTLEN; /* Maximum packet length */
        // k->window   = P_WSLOTS;		/* Maximum window slots */
        k->wslots = 1; /* Current window slots */
        // k->zincnt   = 0;
        // k->dummy    = 0;
        // k->filename = (UCHAR *)0;

        /* Parity must be filled in by the caller */

        // k->retry  = P_RETRY;            /* Retransmission limit */
        // k->s_ctlq = k->r_ctlq = '#';    /* Control prefix */
        k->r_ctlq = '#'; /* Control prefix */
        k->ebq    = 'Y'; /* 8th-bit prefix negotiation */
        k->ebqflg = 0;   /* 8th-bit prefixing flag */
        k->rptq   = '~'; /* Send repeat prefix */
        k->rptflg = 0;   /* Repeat counts negotiated */
        k->s_rpt  = 0;   /* Current repeat count */
        k->capas  = 0;   /* Capabilities */

        k->opktbuf[0] = '\0'; /* No packets sent yet. */
        k->opktlen    = 0;

        return (X_OK);
    } else if (f == K_STATUS) { /* Status report requested. */
        return (X_STATUS);      /* File name, date, size, if any. */

    } else if (f == K_QUIT) { /* You told me to quit */
        return (X_DONE);      /* so I quit. */

    } else if (f == K_ERROR) { /* Send an error packet... */
        epkt(msg, k);
        // k->closef(k,0,(k->state == S_DATA) ? 1 : 2); /* Close file */
        return (X_DONE); /* and quit. */

    } else if (f != K_RUN) { /* Anything else is an error. */
        return (X_ERROR);
    }

    // if (k->state == R_NONE)             /* (probably unnecessary) */
    //     return(X_OK);

    /* If we're in the protocol, check to make sure we got a new packet */

#if ENABLE_DEBUG
    debug(DB_LOG, "r_slot", 0, r_slot);
    debug(DB_LOG, "len", 0, len);
#endif

    k->ipktinfo.len = len; /* Copy packet length to ipktinfo. */

    if (len < 4) {                 /* Packet obviously no good? */
        return (nak(k, k->r_seq)); /* Send NAK for the packet we want */
    }

    /* Parse the packet */

    // if(k->what == W_RECV) /* If we're sending ACKs */
    //{
    //     s = (UCHAR *)0;
    // }

    p = k->ipktbuf; /* Point to it */

    q               = p;                   /* Pointer to data to be checked */
    k->ipktinfo.len = xunchar(*p++);       /* Length field */
    seq = k->ipktinfo.seq = xunchar(*p++); /* Sequence number */
    t = k->ipktinfo.typ = *p++;            /* Type */

    if ((t == 'N' || t == 'Y')) {
        freerslot(k);
        return (X_OK);
    }

    k->ipktinfo.dat = p; /* Data field, maybe */
    // datalen = k->ipktinfo.len - k->bct - 2; /* Data length */
    // chklen = 1;				/* Block check is always type 1 */
    datalen = k->ipktinfo.len - 3; /* Data length */

    // pbc[0] = p[datalen];
    // pbc[1] = '\0';
    p[datalen] = '\0'; /* and the packet DATA field. */

#define SKIP_CHECK
#ifndef SKIP_CHECK

    ok = (xunchar(*pbc) == chk1(q, k));
    if (!ok) {
        freerslot(k);
        nak(k, k->r_seq);
        return (X_OK);
    }
#endif

    if (t == 'E') { /* (AND CLOSE FILES?) */
        return (X_ERROR);
    }

    prev = k->r_seq - 1; /* Get sequence of previous packet */
    if (prev < 0) {
        prev = 63;
    }

    if (seq != k->r_seq) {
        freerslot(k); /* No, discard it. */

        if (seq == prev) { /* If it's the previous packet again */
#if ENABLE_DEBUG
            debug(DB_LOG, "PREVIOUS PKT RETRIES", 0, (long)(k->ipktinfo.rtr));
#endif

            epkt("Too many retries", k); /* Too may */
            return (X_ERROR);            /* Give up */

            // if (k->ipktinfo.rtr++ > k->retry)
            //{ /* Count retries */
            //     epkt("Too many retries", k); /* Too may */
            //     return(X_ERROR);	/* Give up */
            // } else {			/* Otherwise */
            //     return(resend(k));	/* Send old outbound packet buffer */
            // }
        } else {
            return (nak(k, k->r_seq));
        }
    }

    /* Now we have an incoming packet with the expected sequence number. */

#if ENABLE_DEBUG
    debug(DB_CHR, "Packet OK", 0, t);
    debug(DB_LOG, "State", 0, k->state);
#endif

    switch (k->state) {          /* Kermit protocol state switcher */
    case R_WAIT:                 /* Waiting for the S packet */
        if (t == 'S') {          /* Got it */
            spar(k, p, datalen); /* Set parameters from it */
            rc = rpar(k, 'Y');   /* ACK with my parameters */
#if ENABLE_DEBUG
            debug(DB_LOG, "rpar rc", 0, rc);
#endif
            if (rc != X_OK) {
                return (X_ERROR); /* I/O error, quit. */
            }
            k->state  = R_FILE; /* All OK, switch states */
            r->status = R_FILE;
        } else { /* Wrong kind of packet, send NAK */
            epkt("Unexpected packet type", k);
            rc = X_ERROR;
        }
        freerslot(k); /* Free packet slot */
        return (rc);

    case R_FILE:                                     /* Want an F or B packet */
        if (t == 'F') {                              /* File name */
            if ((rc = decode(k, r, 0, p)) == X_OK) { /* Decode and save */
                k->state = R_ATTR;                   /* Switch to next state */
            }
            r->status = k->state;

#if ENABLE_DEBUG
            debug(DB_LOG, "R_FILE decode rc", 0, rc);
            debug(DB_LOG, "R_FILE FILENAME", r->filename, 0);
#endif
            if (rc == X_OK) { /* All OK so far */
                // r->filedate[0] = '\0';	/* No file date yet */
                // r->filesize = 0L;	/* Or file size */
                r->sofar = 0L; /* Or bytes transferred yet */
                // rc = ack(k, k->r_seq, r->filename); /* so ACK the F packet */
                rc = ack(k, k->r_seq, NUL);
            } else {
                epkt("Filename error", k); /* Error decoding filename */
            }
        } else if (t == 'B') { /* Break, end of transaction */
            freerslot(k);
            rc = (ack(k, k->r_seq, (UCHAR *)0) == X_OK) ? X_DONE : X_ERROR;

        } else {
            epkt("Unexpected packet type", k);
            rc = X_ERROR;
        }
        freerslot(k);
        return (rc);

    case R_ATTR:            /* Want A, D, or Z packet */
        if (t == 'D') {     /* First data packet */
            k->obufpos = 0; /* Initialize output buffer */
            // k->filename = r->filename;
            r->sofar = 0L;
            // if ((rc = (*(k->openf))(k,r->filename, 2)) == X_OK) {
            k->state  = R_DATA; /* Switch to Data state */
            r->status = k->state;
            rc        = decode(k, r, 1, p); /* Write out first data packet */
            freerslot(k);
            //} else {
            //    epkt("File refused or can't be opened", k);
            //    freerslot(k);
            //    return(rc);
            //}
            if (rc == X_OK) {
                rc = ack(k, k->r_seq, NUL);
            } else {
                epkt("Error writing data", k);
            }
            return (rc);
        } else if (t == 'Z') { /* Empty file */
#if ENABLE_DEBUG
            debug(DB_LOG, "R_ATTR empty file", r->filename, 0);
#endif
            k->obufpos = 0; /* Initialize output buffer */
            // k->filename = r->filename;
            r->sofar = 0L; /* Open and close the file */
            // if ((rc = (*(k->openf))(k,r->filename, 2)) == X_OK)
            //{
            k->state = R_FILE;
            rc       = ack(k, k->r_seq, NUL);
            //} else {
            //    epkt("File refused or can't be opened", k);
            //    freerslot(k);
            //    return(rc);
            //}

            r->status = k->state;
            freerslot(k);

        } else {
            epkt("Unexpected packet type", k);
            return (X_ERROR);
        }
        break;

    case R_DATA: /* Want a D or Z packet */
#if ENABLE_DEBUG
        debug(DB_CHR, "R_DATA t", 0, t);
#endif
        if (t == 'D') {              /* Data */
            rc = decode(k, r, 1, p); /* Decode it */
            freerslot(k);
        } else if (t == 'Z') { /* End of file */
#if ENABLE_DEBUG
            debug(DB_CHR, "R_DATA", 0, t);
#endif
            if (k->obufpos > 0) { /* Flush output buffer */
                rc = (*(k->writef))(k, k->obuf, k->obufpos);
#if ENABLE_DEBUG
                debug(DB_LOG, "R_DATA writef rc", 0, rc);
#endif
                r->sofar   += k->obufpos;
                k->obufpos  = 0;
            } else {
                rc = X_OK;
            }

            // if (((rc = (*(k->closef))(k,*p,2)) == X_OK) && (rc == X_OK))
            k->state = R_FILE;

#if ENABLE_DEBUG
            debug(DB_LOG, "R_DATA closef rc", 0, rc);
#endif
            r->status = k->state;
            freerslot(k);
        } else {
            epkt("Unexpected packet type", k);
            return (X_ERROR);
        }
        if (rc == X_OK) {
            rc = ack(k, k->r_seq, NUL);
        } else {
            epkt(t == 'Z' ? "Can't close file" : "Error writing data", k);
        }
        return (rc);

    case R_ERROR: /* Canceled from above */
    default:
        epkt(msg, k);
        return (X_ERROR);
    }

    return (X_ERROR);
}

/* Utility routines */

UCHAR *getrslot(struct k_data *k) /* Find a free packet buffer */
{
    // register int i;
    /*
      Note: We don't clear the retry count here.
      It is cleared only after the NEXT packet arrives, which
      indicates that the other Kermit got our ACK for THIS packet.
    */
    if (k->ipktinfo.len < 1) {
        k->ipktinfo.len = -1; /* Mark it as allocated but not used */
        k->ipktinfo.seq = -1;
        k->ipktinfo.typ = SP;
        k->ipktinfo.dat = (UCHAR *)0;
        return (k->ipktbuf);
    }

    return ((UCHAR *)0);
}

void freerslot(struct k_data *k) /* Initialize a window slot */
{
    k->ipktinfo.len = 0; /* Packet length */
}

/*  C H K 1  --  Compute a type-1 Kermit 6-bit checksum.  */

STATIC int chk1(UCHAR *pkt, struct k_data *k)
{
    register unsigned int chk;
    chk = chk2(pkt, k);
    chk = (((chk & 0300) >> 6) + chk) & 077;
    return ((int)chk);
}

/*  C H K 2  --  Numeric sum of all the bytes in the packet, 12 bits.  */

STATIC USHORT chk2(UCHAR *pkt, struct k_data *k)
{
    register USHORT chk = 0, m;
    for (chk = 0; *pkt != '\0'; pkt++) {
        chk += *pkt;
    }
    return (chk);
}

/*   S P K T  --  Send a packet.  */
/*
  Call with packet type, sequence number, data length, data, Kermit struct.
  Returns:
    X_OK on success
    X_ERROR on i/o error
*/
STATIC int spkt(char typ, short seq, int len, UCHAR *data, struct k_data *k)
{
    // unsigned int crc;                   /* For building CRC */
    int i, j, lenpos; /* Workers */
    UCHAR *s, *buf;

#if ENABLE_DEBUG
    debug(DB_LOG, "spkt len 1", 0, len);
#endif

    if (len < 0) { /* Calculate data length ourselves? */
        len = 0;
        s   = data;
        while (*s++) {
            len++;
        }
    }
#if ENABLE_DEBUG
    debug(DB_LOG, "spkt len 2", 0, len);
#endif
    buf = k->opktbuf; /* Where to put packet (FOR NOW) */

    i        = 0;           /* Packet buffer position */
    buf[i++] = SOH;         /* SOH */
    lenpos   = i++;         /* Remember this place */
    buf[i++] = tochar(seq); /* Sequence number */
    buf[i++] = typ;         /* Packet type */
    j        = len + k->bct;
#ifdef F_LP
    if ((len + k->bct + 2) > 94) {    /* If long packet */
        buf[lenpos] = tochar(0);      /* Put blank in LEN field */
        buf[i++]    = tochar(j / 95); /* Make extended header: Big part */
        buf[i++]    = tochar(j % 95); /* and small part of length. */
        buf[i]      = NUL;            /* Terminate for header checksum */
        buf[i++] = tochar(chk1(&buf[lenpos], k)); /* Insert header checksum */
    } else {                                      /* Short packet */
#endif                                            /* F_LP */
        buf[lenpos] = tochar(j + 2); /* Single-byte length in LEN field */
#ifdef F_LP
    }
#endif          /* F_LP */
    if (data) { /* Copy data, if any */
        for (; len--; i++) {
            buf[i] = *data++;
        }
    }
    buf[i] = '\0';

#ifdef F_CRC
    switch (k->bct) { /* Add block check */
    case 1:           /* 1 = 6-bit chksum */
        buf[i++] = tochar(chk1(&buf[lenpos], k));
        break;
    case 2: /* 2 = 12-bit chksum */
        j = chk2(&buf[lenpos], k);
#ifdef XAC
        /* HiTech's XAC compiler silently ruins the regular code. */
        /* An intermediate variable provides a work-around. */
        /* 2004-06-29 -- JHD */
        {
            USHORT jj;
            jj       = (j >> 6) & 077;
            buf[i++] = tochar(jj);
            jj       = j & 077;
            buf[i++] = tochar(jj);
        }
#else
        buf[i++] = (unsigned)tochar((j >> 6) & 077);
        buf[i++] = (unsigned)tochar(j & 077);
#endif /* XAC */
        break;
    case 3: /* 3 = 16-bit CRC */
        crc = chk3(&buf[lenpos], k);
#ifdef XAC
        /* HiTech's XAC compiler silently ruins the above code. */
        /* An intermediate variable provides a work-around. */
        /* 2004-06-29 -- JHD */
        {
            USHORT jj;
            jj       = (crc >> 12) & 0x0f;
            buf[i++] = tochar(jj);
            jj       = (crc >> 6) & 0x3f;
            buf[i++] = tochar(jj);
            jj       = crc & 0x3f;
            buf[i++] = tochar(jj);
        }
#else
        buf[i++] = (unsigned)tochar(((crc & 0170000)) >> 12);
        buf[i++] = (unsigned)tochar((crc >> 6) & 077);
        buf[i++] = (unsigned)tochar(crc & 077);
#endif /* XAC */
        break;
    }
#else
    buf[i++] = tochar(chk1(&buf[lenpos], k));
#endif /* F_CRC */

    buf[i++] = k->s_eom; /* Packet terminator */
    buf[i]   = '\0';     /* String terminator */
    k->s_seq = seq;      /* Remember sequence number */

    k->opktlen = i; /* Remember length for retransmit */

#ifdef DEBUG
    /* CORRUPT THE PACKET SENT BUT NOT THE ONE WE SAVE */
    if (xerror()) {
        UCHAR p[P_PKTLEN + 8];
        int i;
        for (i = 0; i < P_PKTLEN; i++) {
            if (!(p[i] = buf[i])) {
                break;
            }
        }
        p[i - 2] = 'X';
        debug(DB_PKT, "XPKT", (char *)&p[1], 0);
        return ((*(k->txd))(k, p, k->opktlen)); /* Send it. */
    }
    debug(DB_PKT, "SPKT", (char *)&buf[1], 0);
#endif /* DEBUG */

    return ((*(k->txd))(k, buf, k->opktlen)); /* Send it. */
}

/*  N A K  --  Send a NAK (negative acknowledgement)  */

STATIC int nak(struct k_data *k, short seq)
{
    int rc;
    rc = spkt('N', seq, 0, (UCHAR *)0, k);
    // if (k->ipktinfo.rtr++ > k->retry)
    //   rc = X_ERROR;
    return (rc);
}

/*  A C K  --  Send an ACK (positive acknowledgement)  */

STATIC int ack(struct k_data *k, short seq, UCHAR *text)
{
    int len, rc;
    len = 0;
    if (text) { /* Get length of data */
        UCHAR *p;
        p = text;
        for (; *p++; len++)
            ;
    }
    rc = spkt('Y', seq, len, text, k); /* Send the packet */
#if ENABLE_DEBUG
    debug(DB_LOG, "ack spkt rc", 0, rc);
#endif
    if (rc == X_OK) {                   /* If OK */
        k->r_seq = (k->r_seq + 1) % 64; /* bump the packet number */
    }
    return (rc);
}

/*  S P A R  --  Set parameters requested by other Kermit  */

STATIC void spar(struct k_data *k, UCHAR *s, int datalen)
{
    // int x, y;
    // UCHAR c;
    int y;

    s--; /* Line up with field numbers. */

    if (datalen >= 1) { /* Max packet length to send */
        k->s_maxlen = xunchar(s[1]);
    }

    // if (datalen >= 2)                   /* Timeout on inbound packets */
    //   k->r_timo = xunchar(s[2]);

    /* No padding */

    if (datalen >= 5) { /* Outbound Packet Terminator */
        k->s_eom = xunchar(s[5]);
    }

    if (datalen >= 6) { /* Incoming control prefix */
        k->r_ctlq = s[6];
    }

    if (datalen >= 7) { /* 8th bit prefix */
        k->ebq = s[7];
        if ((s[7] > 32 && s[7] < 63) || (s[7] > 95 && s[7] < 127)) {
            if (!k->parity) {  /* They want it */
                k->parity = 1; /* Set parity to something nonzero */
            }
            k->ebqflg = 1;
        } else if (s[7] == 'Y' && k->parity) {
            k->ebqflg = 1;
            k->ebq    = '&';
        } else if (s[7] == 'N') {
            /* WHAT? */
        }
    }
    if (datalen >= 8) { /* Block check */
        k->bct = s[8] - '0';
#ifdef F_CRC
        if ((k->bct < 1) || (k->bct > 3))
#endif /* F_CRC */
            k->bct = 1;
        if (k->bctf) {
            k->bct = 3;
        }
    }
    if (datalen >= 9) { /* Repeat counts */
        if ((s[9] > 32 && s[9] < 63) || (s[9] > 95 && s[9] < 127)) {
            k->rptq   = s[9];
            k->rptflg = 1;
        }
    }

    if (datalen >= 10) { /* Capability bits */
                         //    x = xunchar(s[10]);

#ifdef F_LP /* Long packets */
        if (!(x & CAP_LP))
#endif /* F_LP */
            k->capas &= ~CAP_LP;

#ifdef F_SW /* Sliding Windows */
        if (!(x & CAP_SW))
#endif /* F_SW */
            k->capas &= ~CAP_SW;

#ifdef F_AT /* Attributes */
        if (!(x & CAP_AT))
#endif /* F_AT */
            k->capas &= ~CAP_AT;

#ifdef F_RS /* Recovery */
        if (!(x & CAP_RS))
#endif /* F_RS */
            k->capas &= ~CAP_RS;

#ifdef F_LS /* Locking shifts */
        if (!(x & CAP_LS))
#endif /* F_LS */
            k->capas &= ~CAP_LS;

        /* In case other Kermit sends addt'l capas fields ... */

        for (y = 10; (xunchar(s[y]) & 1) && (datalen >= y); y++)
            ;
    }

#ifdef F_LP /* Long Packets */
    if (k->capas & CAP_LP) {
        if (datalen > y + 1) {
            x           = xunchar(s[y + 2]) * 95 + xunchar(s[y + 3]);
            k->s_maxlen = (x > P_PKTLEN) ? P_PKTLEN : x;
            if (k->s_maxlen < 10) {
                k->s_maxlen = 60;
            }
        }
    }
#endif /* F_LP */

#if ENABLE_DEBUG
    debug(DB_LOG, "S_MAXLEN", 0, k->s_maxlen);
#endif

#ifdef F_SW
    if (k->capas & CAP_SW) {
        if (datalen > y) {
            x         = xunchar(s[y + 1]);
            k->window = (x > P_WSLOTS) ? P_WSLOTS : x;
            if (k->window < 1) { /* Watch out for bad negotiation */
                k->window = 1;
            }
            if (k->window > 1) {
                if (k->window > k->retry) {   /* Retry limit must be greater */
                    k->retry = k->window + 1; /* than window size. */
                }
            }
        }
    }
#endif /* F_SW */
}

/*  R P A R  --  Send my parameters to other Kermit  */

STATIC int rpar(struct k_data *k, char type)
{
    UCHAR *d;
    // int rc, len;
    int rc;
#ifdef F_CRC
    short b;
#endif /* F_CRC */

    d    = k->ack_s;                      /* Where to put it */
    d[0] = tochar(94);                    /* Maximum short-packet length */
    d[1] = tochar(k->s_timo);             /* When I want to be timed out */
    d[2] = tochar(0);                     /* How much padding I need */
    d[3] = ctl(0);                        /* Padding character I want */
    d[4] = tochar(CR);                    /* End-of message character I want */
    d[5] = '#';                           /* Control prefix I send */
    if ((k->ebq == 'Y') && (k->parity)) { /* 8th-bit prefix */
        d[6] = k->ebq = '&';              /* I need to request it */
    } else { /* else just agree with other Kermit */
        d[6] = k->ebq;
    }
    if (k->bctf) {  /* Block check type */
        d[7] = '5'; /* FORCE 3 */
    } else {
        d[7] = k->bct + '0'; /* Normal */
    }
    d[8] = k->rptq;          /* Repeat prefix */
    d[9] = tochar(k->capas); /* Capability bits */
    // d[10] = tochar(k->window);          /* Window size */
    d[10] = tochar(1); /* Window size */

#ifdef F_LP
    d[11] = tochar(k->r_maxlen / 95); /* Long packet size, big part */
    d[12] = tochar(k->r_maxlen % 95); /* Long packet size, little part */
    d[13] = '\0';                     /* Terminate the init string */
    len   = 13;
#else
    d[11] = '\0';
    // len = 11;
#endif /* F_LP */

#ifdef F_CRC
    if (!(k->bctf)) { /* Unless FORCE 3 */
        b      = k->bct;
        k->bct = 1; /* Always use block check type 1 */
    }
#endif /* F_CRC */

    if (type == 'Y') {
        rc = ack(k, 0, d);
    } else {
        rc = -1;
    }

    // switch (type) {
    //   case 'Y':				/* This is an ACK for packet 0 */
    // rc = ack(k,0,d);
    // break;
    // case 'S':				/* It's an S packet */
    // rc = spkt('S', 0, len, d, k);
    // break;
    // default:
    // rc = -1;
    // }

#ifdef F_CRC
    if (!(k->bctf)) { /* Unless FORCE 3 */
        k->bct = b;
    }
#endif           /* F_CRC */
    return (rc); /* Pass along return code. */
}

/*  D E C O D E  --  Decode data field of Kermit packet - binary mode only */
/*
  Call with:
    k = kermit data structure
    r = kermit response structure
    f = function code
      0 = decode filename
      1 = decode file data
    inbuf = pointer to packet data to be decoded
  Returns:
    X_OK on success
    X_ERROR if output function fails
*/
STATIC int decode(struct k_data *k, struct k_response *r, short f, UCHAR *inbuf)
{
    register unsigned int a, a7; /* Current character */
    unsigned int b8;             /* 8th bit */
    int rpt;                     /* Repeat count */
    int rc;                      /* Return code */
    UCHAR *p;

    rc  = X_OK;
    rpt = 0;      /* Initialize repeat count. */
    if (f == 0) { /* Output function... */
        p = r->filename;
    }

    while ((a = *inbuf++ & 0xFF) != '\0') { /* Character loop */
        if (k->rptflg && a == k->rptq) {    /* Got a repeat prefix? */
            rpt = xunchar(*inbuf++ & 0xFF); /* Yes, get the repeat count, */
            a   = *inbuf++ & 0xFF; /* and get the prefixed character. */
        }
        b8 = 0;                           /* 8th-bit value */
        if (k->parity && (a == k->ebq)) { /* Have 8th-bit prefix? */
            b8 = 0200;                    /* Yes, flag the 8th bit */
            a  = *inbuf++ & 0x7F;         /* and get the prefixed character. */
        }
        if (a == k->r_ctlq) {     /* If control prefix, */
            a  = *inbuf++ & 0xFF; /* get its operand */
            a7 = a & 0x7F;        /* and its low 7 bits. */
            if ((a7 >= 0100 && a7 <= 0137) || a7 == '?') { /* Controllify */
                a = ctl(a); /* if in control range. */
            }
        }
        a |= b8; /* OR in the 8th bit */

        if (rpt == 0) {
            rpt = 1; /* If no repeats, then one */
        }

        for (; rpt > 0; rpt--) { /* Output the char 'rpt' times */
            if (f == 0) {
                *p++ = (UCHAR)a;                  /* to memory */
            } else {                              /* or to file */
                k->obuf[k->obufpos++] = (UCHAR)a; /* Deposit the byte */
                if (k->obufpos == k->obuflen) {   /* Buffer full? */
                    rc = (*(k->writef))(k, k->obuf, k->obuflen); /* Dump it. */
                    r->sofar += k->obuflen;
                    if (rc != X_OK) {
                        break;
                    }
                    k->obufpos = 0;
                }
            }
        }
    }
    if (f == 0) {  /* If writing to memory */
        *p = '\0'; /* terminate the string */
    }
    return (rc);
}

/*  E P K T  --  Send a (fatal) Error packet with the given message  */
STATIC void epkt(char *msg, struct k_data *k)
{
    if (!(k->bctf)) { /* Unless FORCE 3 */
        k->bct = 1;
    }
    (void)spkt('E', 0, -1, (UCHAR *)msg, k);
}
#endif /* MIDDLEWARE_WISE_KERMIT */
