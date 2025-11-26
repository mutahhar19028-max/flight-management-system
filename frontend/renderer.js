let currentCustomerId = null;

$(document).ready(function () {
  console.log("=== renderer.js loaded ===");
  window.api.logAction("renderer.js: Script initialized");

  initializeCustomerId();

  const checkBookingLoaded = setInterval(() => {
    if ($("#booking").children().length > 0) {
      clearInterval(checkBookingLoaded);
      console.log("✓ booking.html DOM ready");
      window.api.logAction("renderer.js: booking.html DOM ready");
      attachBookingEvents();
    }
  }, 100);
});

async function initializeCustomerId() {
  currentCustomerId = await window.api.getCustomerId();
  console.log("Generated Customer ID:", currentCustomerId);
  window.api.logAction(`renderer.js: Generated Customer ID: ${currentCustomerId}`);
}

function updateCustomerIdDisplay() {
  $("#current-customer-id").text(currentCustomerId);
  $("#mybookings-customer-id").text(currentCustomerId);
}

function attachBookingEvents() {
  const bookingContainer = $("#booking");
  
  updateCustomerIdDisplay();

  bookingContainer.on("click", "#change-customer-btn", async (e) => {
    e.preventDefault();
    currentCustomerId = await window.api.getCustomerId();
    updateCustomerIdDisplay();
    window.api.logAction(`renderer.js: Changed to new Customer ID: ${currentCustomerId}`);
    alert(`New Customer ID: ${currentCustomerId}`);
  });

  bookingContainer.on("click", "#book-flight-btn", async (e) => {
    e.preventDefault();
    console.log("=== Book Flight button clicked ===");
    window.api.logAction("renderer.js: Book Flight button clicked");

    try {
      const destination = bookingContainer.find("#destination-input").val();
      const flightNumber = bookingContainer.find("#flight-number-input").val();
      const ticketType = bookingContainer.find("#ticket-type-select").val();

      if (!destination || !flightNumber) {
        alert("Please fill in destination and flight number!");
        return;
      }

      const cmd = {
        action: "reserveSeat",
        customerId: currentCustomerId,
        flightNumber: parseInt(flightNumber),
        ticketType: ticketType
      };

      window.api.logAction(`renderer.js: Sending reserve command: ${JSON.stringify(cmd)}`);
      console.log("Sending command:", cmd);

      const res = await window.api.cppAction(cmd);

      console.log("Backend response:", res);
      window.api.logAction(`renderer.js: Reserve response: ${JSON.stringify(res)}`);

      if (res.success) {
        alert(`Booking successful!\nTicket Number: ${res.ticketNumber || 'N/A'}\n${res.message || ''}`);
        bookingContainer.find("#destination-input").val("");
        bookingContainer.find("#flight-number-input").val("");
      } else {
        alert(res.message || "Booking failed");
      }
    } catch (err) {
      console.error("Booking error:", err);
      window.api.logAction(`renderer.js: ERROR during booking - ${err.message || err}`);
      alert("Booking failed: " + (err.message || JSON.stringify(err)));
    }
  });

  bookingContainer.on("click", "#show-database-btn", async (e) => {
    e.preventDefault();
    console.log("=== Show Database button clicked ===");
    window.api.logAction("renderer.js: Show Database button clicked");

    try {
      await window.api.showDatabase();
      console.log("Navigation to database.html initiated");
    } catch (err) {
      console.error("Show database error:", err);
      alert("Failed to show database: " + (err.message || err));
    }
  });

  bookingContainer.on("click", "#refresh-bookings-btn", async (e) => {
    e.preventDefault();
    console.log("=== Refresh Bookings button clicked ===");
    
    try {
      const cmd = {
        action: "getCustomerTickets",
        customerId: currentCustomerId
      };

      const res = await window.api.cppAction(cmd);
      
      if (res.success && res.tickets) {
        const listDiv = $("#bookings-list");
        listDiv.empty();
        
        if (res.tickets.length === 0) {
          listDiv.html("<p>No bookings found for this customer.</p>");
        } else {
          res.tickets.forEach(ticket => {
            listDiv.append(`
              <div style="border: 1px solid #ddd; padding: 10px; margin-bottom: 10px; border-radius: 5px;">
                <strong>Ticket #${ticket.ticketNumber}</strong><br>
                Flight: ${ticket.flightNumber} | Type: ${ticket.ticketType}<br>
                Cost: $${ticket.cost} | Seat: ${ticket.seatNumber}
              </div>
            `);
          });
        }
      } else {
        $("#bookings-list").html("<p>Failed to load bookings.</p>");
      }
    } catch (err) {
      console.error("Refresh bookings error:", err);
      alert("Failed to refresh bookings: " + (err.message || err));
    }
  });

  bookingContainer.on("click", "#update-booking-btn", async (e) => {
    e.preventDefault();
    console.log("=== Update Booking button clicked ===");

    try {
      const ticketNumber = bookingContainer.find("#update-ticket-number").val();
      const newFlightNumber = bookingContainer.find("#update-new-flight").val();
      const newTicketType = bookingContainer.find("#update-new-ticket-type").val();

      if (!ticketNumber || !newFlightNumber) {
        alert("Please enter ticket number and new flight number!");
        return;
      }

      const cmd = {
        action: "changeReservation",
        ticketNumber: parseInt(ticketNumber),
        newFlightNumber: parseInt(newFlightNumber),
        newTicketType: newTicketType
      };

      window.api.logAction(`renderer.js: Sending update command: ${JSON.stringify(cmd)}`);
      const res = await window.api.cppAction(cmd);

      console.log("Update response:", res);
      window.api.logAction(`renderer.js: Update response: ${JSON.stringify(res)}`);

      if (res.success) {
        alert(res.message || "Update successful!");
        bookingContainer.find("#update-ticket-number").val("");
        bookingContainer.find("#update-new-flight").val("");
      } else {
        alert(res.message || "Update failed");
      }
    } catch (err) {
      console.error("Update error:", err);
      alert("Update failed: " + (err.message || err));
    }
  });

  bookingContainer.on("click", "#cancel-booking-btn", async (e) => {
    e.preventDefault();
    console.log("=== Cancel Booking button clicked ===");

    try {
      const ticketNumber = bookingContainer.find("#cancel-ticket-number").val();

      if (!ticketNumber) {
        alert("Please enter ticket number!");
        return;
      }

      const confirmCancel = confirm(`Are you sure you want to cancel ticket ${ticketNumber}?`);
      if (!confirmCancel) return;

      const cmd = {
        action: "cancelReservation",
        ticketNumber: parseInt(ticketNumber)
      };

      window.api.logAction(`renderer.js: Sending cancel command: ${JSON.stringify(cmd)}`);
      const res = await window.api.cppAction(cmd);

      console.log("Cancel response:", res);
      window.api.logAction(`renderer.js: Cancel response: ${JSON.stringify(res)}`);

      if (res.success) {
        alert(res.message || "Cancellation successful!");
        bookingContainer.find("#cancel-ticket-number").val("");
      } else {
        alert(res.message || "Cancellation failed");
      }
    } catch (err) {
      console.error("Cancel error:", err);
      alert("Cancellation failed: " + (err.message || err));
    }
  });

  console.log("✓ All event listeners attached");
  window.api.logAction("renderer.js: All event listeners attached");
}