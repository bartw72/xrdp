/*
Copyright 2005-2013 Jay Sorg

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

composite(alpha blending) calls

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* this should be before all X11 .h files */
#include <xorg-server.h>

/* all driver need this */
#include <xf86.h>
#include <xf86_OSproc.h>

#include <picture.h>

#include "rdp.h"
#include "rdpComposite.h"
#include "rdpDraw.h"

/******************************************************************************/
#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

/******************************************************************************/
static void
rdpCompositeOrg(PictureScreenPtr ps, rdpPtr dev,
                CARD8 op, PicturePtr pSrc, PicturePtr pMask, PicturePtr pDst,
                INT16 xSrc, INT16 ySrc, INT16 xMask, INT16 yMask,
                INT16 xDst, INT16 yDst, CARD16 width, CARD16 height)
{
    ps->Composite = dev->Composite;
    ps->Composite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask,
                  xDst, yDst, width, height);
    ps->Composite = rdpComposite;
}

/******************************************************************************/
void
rdpComposite(CARD8 op, PicturePtr pSrc, PicturePtr pMask, PicturePtr pDst,
             INT16 xSrc, INT16 ySrc, INT16 xMask, INT16 yMask, INT16 xDst,
             INT16 yDst, CARD16 width, CARD16 height)
{
    ScreenPtr pScreen;
    rdpPtr dev;
    PictureScreenPtr ps;

    LLOGLN(10, ("rdpComposite:"));
    pScreen = pSrc->pDrawable->pScreen;
    dev = rdpGetDevFromScreen(pScreen);
    ps = GetPictureScreen(pScreen);
    rdpCompositeOrg(ps, dev, op, pSrc, pMask, pDst, xSrc, ySrc,
                    xMask, yMask, xDst, yDst, width, height);
}
