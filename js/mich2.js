$(document).ready( function() {
	// Load Printer Locations
	$.getJSON('data/printers.json',function(data){
		for(var i = 0; i < data.locations.length; i++)
		{
		    if(i == 0)
		    {
			$('#locations').append("<div class='loc selected'>"+data.locations[i].location+"</div>");
		    }
		    else
		    {
			$('#locations').append("<div class='loc'>"+data.locations[i].location+"</div>");
		    }
		    

		    // Load Printers
		    var output = "<div id='"+remove_spaces(data.locations[i].location)+"' class='printer-list'>";
		    var printers = data.locations[i].printers;
		    for(var j = 0; j < printers.length; j++)
		    {
			output+= "<div class='printer'>"+
                                        "<span class='name'>"+printers[j].name+"</span> [ "+printers[j].loc+" ]<span class='on'>status</span><br/>"+
                                        "<span class='italic'>No jobs in queue.</span>"+
                                "</div>"+
                                "<div class='details' style='display:none'>"+
	  	 	        "<span class='label'>Color</span> "+translate_color(printers[j].type)+"<br/>"+
			            "<span class='label'>Type</span> "+printers[j].size+"<br/>"+
			    "<span class='label'>Cost</span> "+translate_cost(printers[j].type,printers[j].size)+"<br/>"+
				    "<span class='label'>32 bit</span> //pmanager.win.rpi.edu/"+printers[j].name+"<br/>"+
				    "<span class='label'>64 bit</span> //pmanager64.win.rpi.edu/"+printers[j].name+"<br/>"+
			        "</div><div style='height:10px'></div>";
		    }
		    output += "</div>";
		    // Load Printers
		    

		    // Load Maps
		    
		    // Load Maps
		    $("#content").append(output);

		    if(i > 0) // Hide all inactive location data
		    {
			$("#"+remove_spaces(data.locations[i].location)).css( 'display', 'none' );
			$("#"+remove_spaces(data.locations[i].location)+"-map").css( 'display', 'none' );
		    }
		}
		init();
	});
    });

function remove_spaces(word)
{
    return word.replace(" ","-").toLowerCase();;
}

function translate_color(type)
{
    if(type == "Black")
	return "Black &amp; White";
    else
	return type;
}

function translate_cost(type,size)
{
    if(type == "Black" && size == "Printer")
    {
	return "$0.08/page + $0.02/job";
    }
    else if(type == "Color" && size == "Printer")
    {
	return "$0.20/page + $0.20/job";
    }
    else if(type == "Plain Paper" && size == "Plotter")
    {
	return "$1.25 to $2.25 per ft.";
    }
    else if(type == "Glossy Paper" && size == "Plotter")
    {
	return "$2.00 to $3.00 per ft.";
    }
    else if(type == "Clear Film" && size == "Plotter")
    {
	return "$2.25 to $3.25 per ft.";
    }
    else if(type == "Color Duplex" && size == "Printer")
    {
	return "Single side: $0.20/page + $0.20/job Duplex: $0.30/page + $0.20/job";
    }
    else if(type == "Black Duplex" && size == "Printer")
    {
	return "Single side: $0.08/page + $0.02/job Duplex: $0.12/page + $0.02/job";
    }
    else if(type == "Color Duplex" && size == "B-Printer")
    {
	return "Single side: $0.20/page + $0.20/job Duplex: $0.30/page + $0.20/job";
    }
    else if(type == "Black" && size == "Label Printer")
    {
	return "$0.20/page + $0.20/job";
    }
    else if(type == "Black" && size == "B-Printer")
    {
	return "$0.20/page + $0.10/job";
    }
    else if(type == "Color Transparency" && size == "Printer")
    {
	return "$1.00/page + $0.20/job";
    }
}

function init()
{
	$('div#locations div.loc').click( function() {
		$('div#locations div.loc').each( function( i, obj ) {
			//if any other location is selected
			if( $(obj).hasClass( 'selected' ) ) {
				$(obj).removeClass( 'selected' ); //make it not selected
			}
			//make this objects corresponding content hidden
			var corrId = $(obj).text().replace( ' ', '-' ).toLowerCase();
			$('div#'+corrId).css( 'display', 'none' );
			//make this objects map hidden
			$('div#'+corrId+'-map').css( 'display', 'none' );
		});
		//make this location selected
		$(this).addClass('selected');
		//make this locations corresponding content showing
		corrId = $(this).text().replace( ' ', '-' ).toLowerCase();
		$('div#'+corrId).css( 'display', 'block' );
		//make this objects map showing 
		$('div#'+corrId+'-map').css( 'display', 'block' );
	});

	//slide to show details
	$('div.printer').click( function() {
		$(this).next().slideToggle( 'fast' );
	});

	//click floor selectors to filter maps
	$('div.floor-selector').click( function() {
		var mapId = $(this).text().replace( ' ', '-' );
		var mapClass = $(this).parent().attr('id');
		if( $(this).text() == 'All' ) { //show all maps
			$('div.'+mapClass).each( function( i, obj ) {
				$(obj).show();
			});
		} else {
			$('div.'+mapClass).each( function( i, obj ) {
				if( $(obj).attr('id') == mapId ) { //if user wants this map
					$(obj).show();
				} else {
					$(obj).hide();
				}
			});
		}
	});
};
