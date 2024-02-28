/* File completion functions which would be external, were they used
 * outside of this file.
 */

static CompletionState*    cmpl_init_state        (void);
static void                cmpl_free_state        (CompletionState *cmpl_state);
static gint                cmpl_state_okay        (CompletionState* cmpl_state);
static gchar*              cmpl_strerror          (gint);

static PossibleCompletion* cmpl_completion_matches(gchar           *text_to_complete,
						   gchar          **remaining_text,
						   CompletionState *cmpl_state);

/* Returns a name for consideration, possibly a completion, this name
 * will be invalid after the next call to cmpl_next_completion.
 */
static char*               cmpl_this_completion   (PossibleCompletion*);

/* True if this completion matches the given text.  Otherwise, this
 * output can be used to have a list of non-completions.
 */
static gint                cmpl_is_a_completion   (PossibleCompletion*);

/* True if the completion is a directory
 */
static gint                cmpl_is_directory      (PossibleCompletion*);

/* Obtains the next completion, or NULL
 */
static PossibleCompletion* cmpl_next_completion   (CompletionState*);

/* Updating completions: the return value of cmpl_updated_text() will
 * be text_to_complete completed as much as possible after the most
 * recent call to cmpl_completion_matches.  For the present
 * application, this is the suggested replacement for the user's input
 * string.  You must CALL THIS AFTER ALL cmpl_text_completions have
 * been received.
 */
static gchar*              cmpl_updated_text       (CompletionState* cmpl_state);

/* After updating, to see if the completion was a directory, call
 * this.  If it was, you should consider re-calling completion_matches.
 */
static gint                cmpl_updated_dir        (CompletionState* cmpl_state);

/* Current location: if using file completion, return the current
 * directory, from which file completion begins.  More specifically,
 * the cwd concatenated with all exact completions up to the last
 * directory delimiter('/').
 */
static gchar*              cmpl_reference_position (CompletionState* cmpl_state);

/* backing up: if cmpl_completion_matches returns NULL, you may query
 * the index of the last completable character into cmpl_updated_text.
 */
static gint                cmpl_last_valid_char    (CompletionState* cmpl_state);

/* When the user selects a non-directory, call cmpl_completion_fullname
 * to get the full name of the selected file.
 */
static gchar*              cmpl_completion_fullname (gchar*, CompletionState* cmpl_state);


/* Directory operations. */
static CompletionDir* open_ref_dir         (gchar* text_to_complete,
					    gchar** remaining_text,
					    CompletionState* cmpl_state);
static gboolean       check_dir            (gchar *dir_name, 
					    struct stat *result, 
					    gboolean *stat_subdirs);
static CompletionDir* open_dir             (gchar* dir_name,
					    CompletionState* cmpl_state);
static CompletionDir* open_user_dir        (gchar* text_to_complete,
					    CompletionState *cmpl_state);
static CompletionDir* open_relative_dir    (gchar* dir_name, CompletionDir* dir,
					    CompletionState *cmpl_state);
static CompletionDirSent* open_new_dir     (gchar* dir_name, 
					    struct stat* sbuf,
					    gboolean stat_subdirs);
static gint           correct_dir_fullname (CompletionDir* cmpl_dir);
static gint           correct_parent       (CompletionDir* cmpl_dir,
					    struct stat *sbuf);
static gchar*         find_parent_dir_fullname    (gchar* dirname);
static CompletionDir* attach_dir           (CompletionDirSent* sent,
					    gchar* dir_name,
					    CompletionState *cmpl_state);
static void           free_dir_sent (CompletionDirSent* sent);
static void           free_dir      (CompletionDir  *dir);
static void           prune_memory_usage(CompletionState *cmpl_state);

/* Completion operations */
static PossibleCompletion* attempt_homedir_completion(gchar* text_to_complete,
						      CompletionState *cmpl_state);
static PossibleCompletion* attempt_file_completion(CompletionState *cmpl_state);
static CompletionDir* find_completion_dir(gchar* text_to_complete,
					  gchar** remaining_text,
					  CompletionState* cmpl_state);
static PossibleCompletion* append_completion_text(gchar* text,
						  CompletionState* cmpl_state);
static gint get_pwdb(CompletionState* cmpl_state);
static gint first_diff_index(gchar* pat, gchar* text);
static gint compare_user_dir(const void* a, const void* b);
static gint compare_cmpl_dir(const void* a, const void* b);
static void update_cmpl(PossibleCompletion* poss,
			CompletionState* cmpl_state);

static void gtk_file_selection_class_init    (GtkFileSelectionClass *klass);
static void gtk_file_selection_init          (GtkFileSelection      *filesel);
static void gtk_file_selection_destroy       (GtkObject             *object);
static gint gtk_file_selection_key_press     (GtkWidget             *widget,
					      GdkEventKey           *event,
					      gpointer               user_data);

static void gtk_file_selection_file_button (GtkWidget *widget,
					    gint row, 
					    gint column, 
					    GdkEventButton *bevent,
					    gpointer user_data);

static void gtk_file_selection_dir_button (GtkWidget *widget,
					   gint row, 
					   gint column, 
					   GdkEventButton *bevent,
					   gpointer data);

static void gtk_file_selection_populate      (GtkFileSelection      *fs,
					      gchar                 *rel_path,
					      gboolean               try_complete,
					      gboolean               reset_entry);
static void gtk_file_selection_abort         (GtkFileSelection      *fs);

static void gtk_file_selection_update_history_menu (GtkFileSelection       *fs,
						    gchar                  *current_dir);

static void gtk_file_selection_create_dir (GtkWidget *widget, gpointer data);
static void gtk_file_selection_delete_file (GtkWidget *widget, gpointer data);
static void gtk_file_selection_rename_file (GtkWidget *widget, gpointer data);

static void home_clicked (GtkWidget *widget, gpointer data);
static void desktop_clicked (GtkWidget *widget, gpointer data);
static void documents_clicked (GtkWidget *widget, gpointer data);


static GtkWindowClass *parent_class = NULL;

static char *last_dir = NULL;

/* Saves errno when something cmpl does fails. */
static gint cmpl_errno;
