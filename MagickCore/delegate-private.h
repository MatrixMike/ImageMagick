/*
  Copyright @ 1999 ImageMagick Studio LLC, a non-profit organization
  dedicated to making software imaging solutions freely available.
  
  You may not use this file except in compliance with the License.  You may
  obtain a copy of the License at
  
    https://imagemagick.org/script/license.php
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  MagickCore delegates private methods.
*/
#ifndef MAGICKCORE_DELEGATE_PRIVATE_H
#define MAGICKCORE_DELEGATE_PRIVATE_H

#include "MagickCore/locale_.h"
#include "MagickCore/string_.h"

#if defined(MAGICKCORE_GS_DELEGATE)
#include "ghostscript/iapi.h"
#include "ghostscript/ierrors.h"
#else
typedef struct gsapi_revision_s
{
  const char *product;
  const char *copyright;
  long revision;
  long revisiondate;
} gsapi_revision_t;
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#ifndef gs_main_instance_DEFINED
# define gs_main_instance_DEFINED
typedef struct gs_main_instance_s
  gs_main_instance;
#endif

#if !defined(MagickDLLCall)
#  if defined(MAGICKCORE_WINDOWS_SUPPORT)
#    define MagickDLLCall __stdcall
#  else
#    define MagickDLLCall
#  endif
#endif

typedef struct _GhostInfo
{
  void
    (MagickDLLCall *delete_instance)(gs_main_instance *);

  int
    (MagickDLLCall *exit)(gs_main_instance *);

  int
    (MagickDLLCall *init_with_args)(gs_main_instance *,int,char **);

  int
    (MagickDLLCall *new_instance)(gs_main_instance **,void *);

  int
    (MagickDLLCall *run_string)(gs_main_instance *,const char *,int,int *);

  int
    (MagickDLLCall* set_arg_encoding)(gs_main_instance*, int);

  int
    (MagickDLLCall *set_stdio)(gs_main_instance *,int(MagickDLLCall *)(void *,
      char *,int),int(MagickDLLCall *)(void *,const char *,int),
      int(MagickDLLCall *)(void *,const char *,int));

  int
    (MagickDLLCall *revision)(gsapi_revision_t *, int);
} GhostInfo;

static inline char *SanitizeDelegateString(const char *source)
{
  char
    *sanitize_source;

  const char
    *q;

  char
    *p;

  static char
#if defined(MAGICKCORE_WINDOWS_SUPPORT)
    allowlist[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 "
      "$-_.+!;*(),{}|^~[]`\'><#%/?:@&=";
#else
    allowlist[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 "
      "$-_.+!;*(),{}|\\^~[]`\"><#%/?:@&=";
#endif

  sanitize_source=AcquireString(source);
  p=sanitize_source;
  q=sanitize_source+strlen(sanitize_source);
  for (p+=strspn(p,allowlist); p != q; p+=(ptrdiff_t) strspn(p,allowlist))
    *p='_';
  return(sanitize_source);
}

#if defined(MAGICKCORE_WINDOWS_SUPPORT)
static inline void FormatSanitizedDelegateOption(char *string,
  const size_t length,const char *windows_format,
  const char *magick_unused(non_windows_format),const char *option)
{
  char
    *sanitized_option;

  magick_unreferenced(non_windows_format);
  sanitized_option=SanitizeDelegateString(option);
  (void) FormatLocaleString(string,length,windows_format,sanitized_option);
  sanitized_option=DestroyString(sanitized_option);
}
#else
static inline void FormatSanitizedDelegateOption(char *string,
  const size_t length,const char *magick_unused(windows_format),
  const char *non_windows_format,const char *option)
{
  char
    *sanitized_option;

  magick_unreferenced(windows_format);
  sanitized_option=SanitizeDelegateString(option);
  (void) FormatLocaleString(string,length,non_windows_format,sanitized_option);
  sanitized_option=DestroyString(sanitized_option);
}
#endif

extern MagickPrivate MagickBooleanType
  DelegateComponentGenesis(void);

extern MagickPrivate void
  DelegateComponentTerminus(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
