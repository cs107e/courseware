let selected_student_grade = null;
let student_names = [];
let assignment_name = null;
const DEBUG = true;

async function fetch_data() {
  student_names = await fetch("/students").then((res) => res.json());
  assignment = await fetch("/assignment").then((res) => res.json());
  assignment_name = assignment.name;
}

document.addEventListener("DOMContentLoaded", async () => {
  await fetch_data();
  document.getElementById("title").textContent =
    `${assignment_name}. Yet Another Student Submission (YASS) Grader`;
  populateStudentSelector(student_names);
  showAssignmentDetails(student_names[0]);
  setupAddCommentForm();
  setupDeleteCommentForm();
});

function populateStudentSelector(students) {
  const studentSelector = document.getElementById("studentSelector");
  studentSelector.onchange = () => {
    resetUIElements();
    const student = studentSelector.value;
    showAssignmentDetails(student);
  };
  students.forEach((student, index) => {
    const option = document.createElement("option");
    option.value = student;
    option.textContent = student;
    if (index === 0) {
      studentSelector.innerHTML = "";
      studentSelector.value = student;
    }
    studentSelector.appendChild(option);
  });
}

function setupAddCommentForm() {
  const form = document.getElementById("commentAddForm");
  form.style.display = "block";
  form.addEventListener("submit", async (event) => {
    event.preventDefault(); // Prevent the default form submission

    // Extract form data
    const formData = new FormData(form);
    let formProps = Object.fromEntries(formData);
    formProps.from = parseInt(formProps.from);
    formProps.to = parseInt(formProps.to);

    // Craft the body of the request with the form data
    const selectedFile = document.getElementById("fileReviewSelector").value;
    const selectedStudent = document.getElementById("studentSelector").value;
    let bodyJSON = {
      student: selectedStudent,
      assignment: assignment_name,
      filename: document.getElementById("fileReviewSelector").value,
      comment: formProps,
    };
    const bodyContent = JSON.stringify(bodyJSON);
    await fetch("/submit_comment", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: bodyContent,
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error("Failed to submit comment");
        }
        return response.json();
      })
      .then((_) => {
        // Show success message
        showSuccessMessage("Comment submitted successfully");
      })
      .catch((error) => {
        // Show error in the error-notifier
        showError(error.message);
      });

    form.reset();
    showAssignmentDetails(selectedStudent, selectedFile);
  });
}

function setupDeleteCommentForm() {
  const form = document.getElementById("commentDeleteForm");
  form.addEventListener("submit", async (event) => {
    event.preventDefault();

    const formData = new FormData(form);
    let formProps = Object.fromEntries(formData);
    formProps.from = parseInt(formProps.from);

    const selectedStudent = document.getElementById("studentSelector").value;
    const selectedFile = document.getElementById("fileReviewSelector").value;
    let bodyJSON = {
      student: selectedStudent,
      assignment: assignment_name,
      filename: document.getElementById("fileReviewSelector").value,
      comment_from: formProps.from,
    };

    const bodyContent = JSON.stringify(bodyJSON);
    await fetch("/delete_comment", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: bodyContent,
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error("Failed to delete comment");
        }
        return response.json();
      })
      .then((_) => {
        showSuccessMessage("Comment deleted successfully");
      })
      .catch((error) => {
        showError(error.message);
      });

    form.reset();
    showAssignmentDetails(selectedStudent, selectedFile);
  });
}

async function showAssignmentDetails(student_name, selectedFile = null) {
  try {
    selected_student_grade = await fetch(`${student_name}_grades.json`).then(
      (res) => res.json(),
    );

    if (!selected_student_grade) {
      throw new Error(`Grades not found for ${student_name}`);
    }

    const assignmentDetails = selected_student_grade.grades[assignment_name];
    if (!assignmentDetails) {
      throw new Error(`Assignment details not found for ${assignment_name}`);
    }

    const { name, timeliness, test_totals, test_results, review } =
      assignmentDetails;
    const { submit, extension, retests } = timeliness || {};

    document.getElementById("commentAddForm").style.display = "block";
    populateReviewSection(review, assignment_name, selectedFile);
  } catch (error) {
    // Show a generic error in the error-notifier
    showError(
      `Error occurred fetching assignment details for ${student_name}: ${error}. Have you ran the autograder?`,
    );
  }
}

function populateReviewSection(review, assignmentKey, selectedFile = null) {
  const selector = document.getElementById("fileReviewSelector");
  selector.innerHTML = `<option value="none">No Files</option>`;
  selector.onchange = () => updateReviewCodeMirror(assignmentKey);
  window.updateEditorContent("", []);
  document.getElementById("editor").style.display = "none";

  if (review && Object.entries(review).length > 0) {
    populateReviewFileSelector(review, selector, assignmentKey, selectedFile);
  }
}

function populateReviewFileSelector(
  review,
  selector,
  assignmentKey,
  selectedFile = null,
) {
  Object.keys(review).forEach((fileName, index) => {
    const option = document.createElement("option");
    option.value = fileName;
    option.textContent = fileName;
    if (index === 0) {
      document.getElementById("editor").style.display = "block";
      selector.innerHTML = ""; // Clear existing options
      selector.appendChild(option);
    } else {
      selector.appendChild(option);
    }
  });

  if (selectedFile && review[selectedFile]) {
    selector.value = selectedFile;
    updateReviewCodeMirror(assignmentKey);
  } else if (Object.keys(review).length > 0) {
    selector.value = Object.keys(review)[0];
    updateReviewCodeMirror(assignmentKey);
  }
}

async function updateReviewCodeMirror(assignmentKey) {
  const assignmentDetails = selected_student_grade.grades[assignmentKey];
  if (!assignmentDetails) {
    // Show error in the error-notifier
    showError("Assignment details not found");
    window.updateEditorContent("", []);
    return;
  }
  const selector = document.getElementById("fileReviewSelector");
  const selectedIndex = selector.value;
  if (selectedIndex !== "") {
    const fileReview = assignmentDetails.review[selectedIndex];
    window.updateEditorContent(fileReview.lines, fileReview.comments);
  } else {
    window.updateEditorContent("", []);
  }
}

function showError(message) {
  const errorNotifier = document.getElementById("errorNotifier");
  errorNotifier.textContent = message;
  errorNotifier.style.display = "block";
}

function showSuccessMessage(message) {
  // Implement success message display if needed
}

function resetUIElements() {
  document.getElementById("errorNotifier").style.display = "none";
  document.getElementById("fileReviewSelector").innerHTML =
    `<option value="">No Files</option>`;
  document.getElementById("editor").style.display = "none";
  const commentAddForm = document.getElementById("commentAddForm");
  commentAddForm.reset();
  commentAddForm.style.display = "none";
}
